import json
from enum import Enum
from typing import Optional, List, Tuple

from django.http import HttpRequest
from django.http.response import JsonResponse

from core.models import Board, Player
import core.services.auth

class HttpMethod(Enum):

    GET = 'GET'
    POST = 'POST'


class AuthMethod(Enum):

    SESSION = 'session'
    LINK_TOKEN = 'link_token'
    AUTH_TOKEN = 'auth_token'


class ApiError(Enum):

    METHOD_NOT_ALLOWED = (405, 'Invalid HTTP method')
    UNAUTHORIZED = (401, 'Authentication failed')
    GAME_DOES_NOT_EXIST = (404, 'Game does not exist')
    INVALID_JSON_INPUT = (400, 'Invalid JSON supplied, bad request')
    UNPARSABLE_MOVE = (400, 'Cannot parse the move')
    INVALID_MOVE = (400, 'That is an invalid move')


def authenticate_request(
    request: HttpRequest,
    allow_methods: List[HttpMethod],
    allow_auth: List[AuthMethod]) -> Tuple[Optional[Board], Optional[Player], \
                Optional[JsonResponse]]:
    """
    Authenticates the request and returns an error response and board.
    """
    error_response = None
    board = None
    player = None
    if not HttpMethod(request.method) in allow_methods:
        error_response = make_error_json(error=ApiError.METHOD_NOT_ALLOWED)
        return None, None, error_response
    if request.user.is_authenticated and AuthMethod.SESSION in allow_auth:
        if request.user.is_active:
            board = request.user.player.board_set.first()
            player = request.user.player
    if 'Authorization' in request.headers:
        token = request.headers['Authorization'].split('Bearer ')[-1]
        if ':' in token and AuthMethod.AUTH_TOKEN in allow_auth:
            board = core.services.auth.verify_auth_token(token)
        elif AuthMethod.LINK_TOKEN in allow_auth:
            board = core.services.auth.redeem_link_token(token)
    if not (board or player):
        error_response = make_error_json(error=ApiError.UNAUTHORIZED)
    return board, player or (board.player if board else None), error_response


def make_json(payload: dict, error: Optional[ApiError] = None) -> JsonResponse:
    """
    Creates a JSON response from the payload to be returned to Django.
    """
    status = 200
    payload['chesstastic'] = 'Have a chesstastic day!'
    if error:
        payload['chesstastic'] = 'That is a chessicoulous request!'
        status, description = error.value
        payload['error'] = {
            'code': error.name,
            'description': description,
        }
    payload['status'] = status
    return JsonResponse(payload, status=status)


def make_error_json(error: ApiError) -> JsonResponse:
    """
    Same as make_json() but then for purely the error.
    """
    return make_json({}, error=error)


def get_json(request: HttpRequest) -> Tuple[Optional[dict], Optional[dict]]:
    """
    Take the request and attempt to extract JSON. If this fails, return an error
    response.
    """
    try:
        return json.loads(request.body), None
    except json.JSONDecodeError:
        return None, make_error_json(ApiError.INVALID_JSON_INPUT)
