from typing import Optional
from core.models import Player


def process_elo(p1: Player, p2: Player, winner: Optional[Player]):
    """
    Process the ELO for two players and one winner. Returns new respective ELO values.

    1. Compute expected score of player, for example Expected score of player1 =
       1/(1+10^((Rating2-rating1)/D)) where rating2 is rating of player 2, rating1 is rating of
       player 1 and D is some equalizing value, commonly 400. 
    2. Observe actual score which is 1 if player wins, 0.5 if player draws and 0 if player loses.
    3. Update rating of both players, for player 1: new rating =
       old rating + K*(Actual score - Expected score) where K is some fixed value, commonly 32.
    """
    D = 400
    K = 32
    expected_p1 = 1 / (1 + (10 ** ((p2.elo - p1.elo) / D)))
    expected_p2 = 1 / (1 + (10 ** ((p1.elo - p2.elo) / D)))
    actual_p1 = 0.5
    actual_p2 = 0.5
    if winner == p1:
        actual_p1 = 1
        actual_p2 = 0
    elif winner == p2:
        actual_p1 = 0
        actual_p2 = 1
    p1.elo = int(p1.elo + (K * (actual_p1 - expected_p1)))
    p2.elo = int(p2.elo + (K * (actual_p2 - expected_p2)))
    p1.save()
    p2.save()
    return p1.elo, p2.elo
