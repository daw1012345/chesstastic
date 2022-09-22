import hashlib
import uuid
import secrets
from typing import Optional

from django.db import transaction
from django.utils import timezone


from core.models import Player, Board


def verify_auth_token(token: str) -> Optional[Board]:
    """
    Returns linked board if the authentication token is valid. Assumes token in form of
    "<uid>:<auth_token>". An authentication token is valid if it is the hash of the secret
    token concatenated with the current date and hour.
    """
    uid, token = token.split(':', 1)
    board = Board.objects.filter(uid=uid, player__user__is_active=True).first()
    if board:
        now = timezone.localtime(timezone.now())
        formatted_time = now.strftime('%Y%m%d%H')
        correct_token = hashlib.sha256(
            f'{board.token}{formatted_time}'.encode('utf-8')
        ).hexdigest()
        if correct_token == token:
            return board


@transaction.atomic
def redeem_link_token(token: str) -> Optional[Board]:
    """
    Removes the link token from the player object, then returns a newly
    generated board object. Returns None if the link token is not valid.

    Link tokens are only valid if they are set on a player object, are at least 8 characters
    long, and that player has no other boards linked yet.
    """
    player = Player.objects.filter(link_token=token, user__is_active=True).first()
    if player and player.board_set.all().count() == 0 and len(token) > 7:
        player.link_token = ''
        player.save()
        new_uid = hashlib.sha256(uuid.uuid4().hex.encode('utf-8')).hexdigest()
        new_token = hashlib.sha256(secrets.token_hex(64).encode('utf-8')).hexdigest()
        board = Board(
            player=player,
            uid=new_uid,
            token=new_token,
        )
        board.save()
        return board
