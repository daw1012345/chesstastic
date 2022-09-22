from typing import Optional, Tuple

from django.views.generic import View, TemplateView, FormView, DetailView, ListView
from django.views.generic.edit import UpdateView, ModelFormMixin
from django.http import HttpRequest, JsonResponse
from django.contrib.auth.models import User
from django.contrib.auth.mixins import LoginRequiredMixin
from django.contrib.auth.views import LoginView as DjangoLoginView
from django.contrib.auth.views import LogoutView as DjangoLogoutView
from django.contrib.auth import login
from django.views.decorators.csrf import csrf_exempt
from django.utils.decorators import method_decorator
from django.shortcuts import reverse, redirect

from core.models import Game, Player
from core.forms import LinkForm, UsernameForm, RegistrationForm
import core.services.auth
import core.services.api
import core.services.game
import core.services.gamelogic
from core.forms import GameForm


class HomeView(TemplateView):

    template_name = 'home.html'


class AboutView(TemplateView):

    template_name = 'about.html'


class RegistrationView(FormView):

    template_name = 'registration.html'
    form_class = RegistrationForm

    def form_valid(self, form):
        player = form.save(commit=False)
        user = User.objects.create_user(
            form.cleaned_data['email'],
            form.cleaned_data['password'],
        )
        player.user = user
        player.save()
        login(self.request, user)
        return redirect('dashboard')


class LoginView(DjangoLoginView):

    template_name = 'login.html'
    redirect_authenticated_user = True


class LogoutView(DjangoLogoutView):

    pass


class DashboardView(LoginRequiredMixin, TemplateView):

    template_name = 'dashboard.html'

    def get_context_data(self):
        active_game = core.services.game.get_player_active_game(self.request.user.player)
        return {
            'active_game': active_game,
        }


class GameOverview(LoginRequiredMixin, TemplateView):

    template_name = 'game_overview.html'

    def get_context_data(self):
        return {
            'games': core.services.game.get_player_games(self.request.user.player).exclude(
                game_over=False
            )
        }


class GameView(LoginRequiredMixin, DetailView):

    template_name = 'game.html'
    model = Game


class NewGameView(LoginRequiredMixin, FormView):

    template_name = 'game_new.html'
    
    def get_form(self):
        return GameForm(player=self.request.user.player, **self.get_form_kwargs())

    def form_valid(self, form):
        player = self.request.user.player
        white = form.cleaned_data['color'] == GameForm.WHITE
        game = Game()
        if white:
            game.white = player
            game.black = form.cleaned_data['opponent']
        else:
            game.black = player
            game.white = form.cleaned_data['opponent']
        game.save()
        return redirect('game', pk=game.pk)

    def dispatch(self, request, *args, **kwargs):
        active_game = core.services.game.get_player_active_game(self.request.user.player)
        if active_game:
            return redirect('game', pk=active_game.pk)
        return super().dispatch(request, *args, **kwargs)


class AccountView(LoginRequiredMixin, FormView):

    template_name = 'account.html'
    form_class = LinkForm
    success_url = '.'

    def form_valid(self, form):
        player = self.request.user.player
        player.link_token = form.cleaned_data['link_token']
        player.save()
        return super().form_valid(form)

    def dispatch(self, request):
        if request.method == 'POST' and 'unlink' in request.POST:
            player = request.user.player
            player.link_token = ''
            player.board_set.all().delete()
            player.save()
            return redirect('account')
        return super().dispatch(request)


class UsernameView(LoginRequiredMixin, UpdateView, ModelFormMixin):

    form_class = UsernameForm
    template_name = 'username.html'
    
    def get_success_url(self):
        return reverse('account')

    def get_object(self):
        return self.request.user.player


class LeaderboardView(LoginRequiredMixin, ListView):

    template_name = 'leaderboard.html'
    queryset = Player.objects.all()
    ordering = ['-elo']


class HelpView(TemplateView):

    template_name = 'help.html'


@method_decorator(csrf_exempt, name='dispatch')
class BaseApiView(View):

    allow_methods = []
    allow_auth = []

    def _authenticate(self, request: HttpRequest) -> Optional[JsonResponse]:
        """
        Authenticates the request and sets self.player and self.board. Will return None if
        authentication is succesful and a response otherwise.
        """
        board, player, error_response = core.services.api.authenticate_request(
            request,
            self.allow_methods,
            self.allow_auth,
        )
        if error_response:
            return error_response
        self.board = board
        self.player = player

    def get(self, request, **kwargs):
        return self._authenticate(request) or self.do_get(request, **kwargs)

    def post(self, request, **kwargs):
        return self._authenticate(request) or self.do_post(request, **kwargs)


class ApiLinkView(BaseApiView):

    allow_methods = [core.services.api.HttpMethod.POST]
    allow_auth = [core.services.api.AuthMethod.LINK_TOKEN]

    def do_post(self, request):
        return core.services.api.make_json({
            'uid': self.board.uid,
            'token': self.board.token,
        })


class ApiInfoView(BaseApiView):

    allow_methods = [core.services.api.HttpMethod.GET]
    allow_auth = [
        core.services.api.AuthMethod.AUTH_TOKEN,
        core.services.api.AuthMethod.SESSION,
    ]

    def do_get(self, request):
        return core.services.api.make_json({
            'username': self.board.player.username,
        })


class ApiPollView(BaseApiView):

    IDLE = 'idle'
    GAME = 'game'

    allow_methods = [core.services.api.HttpMethod.GET]
    allow_auth = [core.services.api.AuthMethod.AUTH_TOKEN]

    def do_get(self, request):
        game = core.services.game.get_player_active_game(self.player)
        return core.services.api.make_json({
            'action': self.GAME if game else self.IDLE,
            'game': game.pk if game else None,
        })


class ApiGameView(BaseApiView):

    allow_methods = [
        core.services.api.HttpMethod.GET,
        core.services.api.HttpMethod.POST,
    ]
    allow_auth = [
        core.services.api.AuthMethod.AUTH_TOKEN,
        core.services.api.AuthMethod.SESSION,
    ]

    def _get_game(self, pk) -> Tuple[Optional[Game], Optional[JsonResponse]]:
        try:
            game = core.services.game.get_player_games(self.player).get(pk=pk)
        except Game.DoesNotExist:
            return None, core.services.api.make_error_json(
                core.services.api.ApiError.GAME_DOES_NOT_EXIST
            )
        return game, None

    def do_get(self, request, pk):
        game, error_response = self._get_game(pk)
        player_color = core.services.gamelogic.Color.WHITE if self.player == game.white else \
            core.services.gamelogic.Color.BLACK
        board = core.services.game.get_board_from_game(game)
        return error_response or core.services.api.make_json({
            'board': core.services.game.serialize_board(board),
            'white': game.white.username,
            'black': game.black.username,
            'game_over': game.game_over,
            'winner': game.winner.username if game.winner else None,
            'your_turn': core.services.gamelogic.get_current_color(board) == player_color,
        })

    def do_post(self, request, pk):
        game, error_response = self._get_game(pk)
        body, error_response = core.services.api.get_json(request)
        if game and body:
            if game.game_over:
                return core.services.api.make_json({
                    'invalid_reason': 'Game is not in a mutable state',
                }, error=core.services.api.ApiError.INVALID_MOVE)
            is_white = self.player == game.white
            color = core.services.gamelogic.Color.WHITE if is_white else \
                    core.services.gamelogic.Color.BLACK 
            try:
                move = core.services.game.parse_move(body['move'], color)
                board = core.services.game.get_board_from_game(game)
                invalid_reason = core.services.gamelogic.validate_move(move, board)
                if invalid_reason:
                    error_response = core.services.api.make_json({
                        'invalid_reason': invalid_reason,
                    }, error=core.services.api.ApiError.INVALID_MOVE)
                else:
                    core.services.game.save_turn(
                        game,
                        self.player,
                        move,
                    )
            except (ValueError, KeyError):
                error_response = core.services.api.make_error_json(
                    core.services.api.ApiError.UNPARSABLE_MOVE
                )
        return error_response or self.do_get(request, pk)


class ApiReplayView(BaseApiView):

    allow_methods = [
        core.services.api.HttpMethod.GET,
    ]
    allow_auth = [
        core.services.api.AuthMethod.AUTH_TOKEN,
        core.services.api.AuthMethod.SESSION,
    ]

    def do_get(self, request, pk):
        try:
            game = core.services.game.get_player_games(self.player).get(pk=pk)
        except Game.DoesNotExist:
            return core.services.api.make_error_json(
                core.services.api.ApiError.GAME_DOES_NOT_EXIST
            )
        return core.services.api.make_json(core.services.game.serialize_replay(
            game,
        ))
