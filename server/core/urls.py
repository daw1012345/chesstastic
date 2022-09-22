from django.urls import path

from . import views


urlpatterns = [
    path('', views.HomeView.as_view(), name='home'),
    path('about/', views.AboutView.as_view(), name='about'),
    path('register/', views.RegistrationView.as_view(), name='register'),
    path('login/', views.LoginView.as_view(), name='login'),
    path('logout/', views.LogoutView.as_view(), name='logout'),
    path('dashboard/', views.DashboardView.as_view(), name='dashboard'),
    path('game/', views.GameOverview.as_view(), name='game_overview'),
    path('game/new/', views.NewGameView.as_view(), name='game_new'),
    path('game/<pk>/', views.GameView.as_view(), name='game'),
    path('account/', views.AccountView.as_view(), name='account'),
    path('account/username/', views.UsernameView.as_view(), name='username'),
    path('leaderboard/', views.LeaderboardView.as_view(), name='leaderboard'),
    path('help/', views.HelpView.as_view(), name='help'),
    path('api/link/', views.ApiLinkView.as_view(), name='api_link'),
    path('api/info/', views.ApiInfoView.as_view(), name='api_info'),
    path('api/poll/', views.ApiPollView.as_view(), name='api_poll'),
    path('api/game/<pk>/', views.ApiGameView.as_view(), name='api_game'),
    path('api/game/<pk>/replay/', views.ApiReplayView.as_view(), name='api_replay'),
]
