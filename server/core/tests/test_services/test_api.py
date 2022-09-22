from unittest.mock import patch

from django.test import TestCase, RequestFactory
from django.contrib.auth.models import AnonymousUser, User

from core.models import Player, Board
import core.services.api


class TestAuthenticateRequest(TestCase):

    def setUp(self):
        self.request = RequestFactory().get('/')
        self.request.user = AnonymousUser()
        self.active_user = User.objects.create_user(
            username='mark1',
            email='mark1@boom.com',
            password='$ecret',
        )
        self.inactive_user = User.objects.create_user(
            username='mark2',
            email='mark2@boom.com',
            password='$ecret',
        )
        player1 = Player.objects.create(
            user=self.active_user,
            username='Mark 1',
        )
        player2 = Player.objects.create(
            user=self.inactive_user,
            username='Mark 2',
        )
        Board.objects.create(
            player=player1,
            uid='deadbeef01',
            token='deadbeef01xx',
        )
        Board.objects.create(
            player=player2,
            uid='deadbeef02',
            token='deadbeef02xx',
        )
        self.inactive_user.is_active = False
        self.inactive_user.save()

    def test_session_get_noauth(self):
        """
        GET request, SESSION auth
        No authentication supplied
        Authentication should fail
        """
        board, player, response = core.services.api.authenticate_request(
            self.request,
            [core.services.api.HttpMethod.GET],
            [core.services.api.AuthMethod.SESSION],
        )
        assert board is None
        assert response is not None

    def test_session_get_inactive(self):
        """
        GET request, SESSION auth
        Inactive user supplied
        Authentication should fail
        """
        self.request.user = self.inactive_user
        board, player, response = core.services.api.authenticate_request(
            self.request,
            [core.services.api.HttpMethod.GET],
            [core.services.api.AuthMethod.SESSION],
        )
        assert board is None
        assert response is not None
    
    def test_session_get_active(self):
        """
        GET request, SESSION auth
        Active user supplied
        Authentication should succeed
        """
        self.request.user = self.active_user
        board, player, response = core.services.api.authenticate_request(
            self.request,
            [core.services.api.HttpMethod.GET],
            [core.services.api.AuthMethod.SESSION],
        )
        assert board is not None
        assert response is None
