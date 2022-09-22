from django.db import models
from django.contrib.auth.models import User
    

class Player(models.Model):

    user = models.OneToOneField(User, on_delete=models.CASCADE)
    username = models.CharField(max_length=64, unique=True)
    elo = models.PositiveIntegerField(default=400)
    bot = models.BooleanField(default=False)
    link_token = models.CharField(max_length=256, blank=True, default='')

    def __str__(self):
        return f'{self.username} ({self.elo})'


class Game(models.Model):

    white = models.ForeignKey('core.Player', on_delete=models.CASCADE, related_name='white_game_set')
    black = models.ForeignKey('core.Player', on_delete=models.CASCADE, related_name='black_game_set')

    # Calculated fields
    game_over = models.BooleanField(default=False)
    winner = models.ForeignKey('core.Player', on_delete=models.CASCADE, related_name='won_set',
        null=True, blank=True)

    def __str__(self):
        done = ' (done)' if self.game_over else ''
        return f'Game {self.white.username} vs. {self.black.username}{done}'


class Turn(models.Model):

    REGULAR = 'regular'
    CASTLE_LONG = 'castle_long'
    CASTLE_SHORT = 'castle_short'
    PROMOTION_BISHOP = 'promotion_bishop'
    PROMOTION_QUEEN = 'promotion_queen'
    PROMOTION_KNIGHT = 'promotion_knight'
    PROMOTION_ROOK = 'promotion_rook'
    RESIGNATION = 'resign'
    MOVE_TYPE_CHOICES = [
        (REGULAR, 'Regular'),
        (CASTLE_LONG, 'Castle Long'),
        (CASTLE_SHORT, 'Castle Short'),
        (PROMOTION_BISHOP, 'Promotion Bishop'),
        (PROMOTION_QUEEN, 'Promotion Queen'),
        (PROMOTION_KNIGHT, 'Promotion Knight'),
        (PROMOTION_ROOK, 'Promotion Rook'),
        (RESIGNATION, 'Player resigned'),
    ]

    game = models.ForeignKey('core.Game', on_delete=models.CASCADE)
    player = models.ForeignKey('core.Player', on_delete=models.CASCADE)
    move_type = models.CharField(choices=MOVE_TYPE_CHOICES, max_length=32)
    source_field = models.PositiveIntegerField(null=True, blank=True)
    destination_field = models.PositiveIntegerField(null=True, blank=True)

    def __str__(self):
        source = self.source_field if self.source_field else '?'
        destination = self.destination_field if self.destination_field else '?'
        return f'Turn {self.game.pk}/{self.pk} {source}=>{destination}'


class Board(models.Model):

    player = models.ForeignKey('core.Player', unique=True, on_delete=models.CASCADE)
    uid = models.CharField(max_length=256, unique=True)
    token = models.CharField(max_length=256, unique=True)

    def __str__(self):
        return f'Board from {self.player}'

    def get_short_uid(self):
        return self.uid[:8]
