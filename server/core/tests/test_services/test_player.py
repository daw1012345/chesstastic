from unittest.mock import patch

from django.test import TestCase

from core.models import Player
import core.services.player


@patch('core.models.Player.save', return_value=None)
class ProcessElo(TestCase):

    def setUp(self):
        self.p1 = Player(elo=400)
        self.p2 = Player(elo=400)

    def test_p1_wins(self, mock):
        p1_elo, p2_elo = core.services.player.process_elo(self.p1, self.p2, self.p1)
        assert p1_elo == 416
        assert p2_elo == 384

    def test_p1_draws(self, mock):
        p1_elo, p2_elo = core.services.player.process_elo(self.p1, self.p2, None)
        assert p1_elo == 400
        assert p2_elo == 400

    def test_commutative(self, mock):
        """
        Makes sure the calculation is the same both directions.
        """
        a, b = core.services.player.process_elo(self.p1, self.p2, self.p1)
        self.setUp()
        c, d = core.services.player.process_elo(self.p1, self.p2, self.p2)
        print(a, b, c, d)
        assert a == d
        assert b == c        
