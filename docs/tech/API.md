# Chesstastic API documentation

This document aims to specify the functionalities and working of the Chesstastic API.

## Authentication

Chesstastic supports three ways of authenticating:

* Through a session cookie (`S`)  
Set this by logging in into a web browser.
* Via an authentication token (`A`)  
This can be generated on the client side, if you know a `secret`.
* Via a link token (`L`)  
Temporary token, used to obtain the `secret`.

The `A` and `L` authentication schemes make use of a `Bearer`-token.

Per API points different authentication methods differ.

**Authentication token generation**  
Generated like `sha256(secret + now.year + now.month + now.day + now.hour)` where the year is
written in full (`2021`), months are 1-12, days start at 1 and hours are 1-23. Timezone CET, no
leading zeros, encoded as UTF-8. Prepend your UID and seperate the two components using a `:`.

**Linking process**
A user can set the link token you give to them in their account. Then use that link token on
the `/api/link/` endpoint to obtain a `secret`.

## Errors

The API will either respond with an HTTP `200` status code or an error response. An error response
will always include the following fields, unless it's an HTTP `500`:  
```jsonc
{
    "error": {
        {
            "code": "<name of error>",
            "description": "<short description>"
        }
    }
}
```

The default list of possible errors is:
* `500` | In case the server breaks
* `401` | Invalid authentication

Further fields or status codes may be available, please refer to the endpoint documentation
for that.

## `/api/link/` &ndash; Linking
Link a board.

**Authentication**: `L`

**POST**  
Request body: _empty_  
Response body:
```jsonc
{
    "uid": "<your UID>",
    "token": "<your secret>"
}
```
Errors: _none_

## `/api/info/` &ndash; User info
Get user information.

**Authentication**: `A|S`

**GET**  
Response body:
```jsonc
{
    "username": "<your username>"
}
```
Errors: _none_

## `/api/poll/` &ndash; Action polling
Describes what the board should do next.

**Authentication**: `A`

**GET**  
Response body:
```jsonc
{
    "action": "idle|game",
    "game": 1, // nullable <ID of the game>
}
```
Errors: _none_

## `/api/game/{id}/` &ndash; Game management
Get games, make moves.

**Authentication**: `A|S`

**GET**  
Response body:
```jsonc
{
    "board": {
        "state": [
            {
                "piece": { // nullable
                    "type": "king|pawn|...",
                    "color": "black|white"
                }, 
                "position": {
                    "index": 1, // <1-64, index field>
                    "coordinates": {"row": 1, "col": 1},
                    "algebraic": "A1|A2|A3|..."
                }
            }
        ],
        "moves": [
            {
                "color": "black|white",
                "source": 1, // nullable <1-64, index field>
                "target": 1, // nullable <1-64, index field>
                "move_type": "regular|castle_left|..."
            }
        ]
    },
    "white": "<whites' username>",
    "black": "<blacks' username>",
    "game_over": false,
    "winner": "<winners username>", // nullable
    "your_turn": false,
}
```
Errors:
* `404` | If the game does not exist

**POST**  
Request body:
```jsonc
{
    "move": {
        "source": 1, // nullable <1-64, index field>
        "target": 1, // nullable <1-64, index field>
        "move_type": "regular|castle_short|..."
    }
}
```
Response body: _identical to GET_
Errors:
* `404` | If the game does not exist
* `400` | If the move is invalid, additional field `invalid_reason` will be supplied

## `/api/game/{id}/replay/` &ndash; Replay a game
Return a full replay of the game.

**Authentication**: `A|S`

**GET**  
Response body:
```jsonc
{
    "steps": [
        "state": [
            {
                "piece": { // nullable
                    "type": "king|pawn|...",
                    "color": "black|white"
                },
            } // <for every position on the board>
        ],
        "best_move": {
            "algebraic": "<best move in algebraic notation>",
            "source_index": 1, // <source of the best move>
            "target_index": 1 // <target of the best move>
        }
    ]
}
```
Errors:
* `404` | If the game does not exist
