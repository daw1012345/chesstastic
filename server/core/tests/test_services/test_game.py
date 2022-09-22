from django.test import TestCase

import core.services.game
import core.services.gamelogic


class StringboardToBoard(TestCase):

    def test_service(self):
        stringboard = """
        RKBQXBKR
        PPPPPPPP
        ........
        ........
        ........
        ........
        pppppppp
        rkbqxbkr
        """
        board = core.services.game.stringboard_to_board(stringboard)
        state = board.state
        assert state[core.services.gamelogic.Position(1)] == core.services.gamelogic.Piece(
            type=core.services.gamelogic.PieceType.ROOK,
            color=core.services.gamelogic.Color.WHITE,
        )
        assert state[core.services.gamelogic.Position(4)] == core.services.gamelogic.Piece(
            type=core.services.gamelogic.PieceType.QUEEN,
            color=core.services.gamelogic.Color.WHITE,
        )
        assert state[core.services.gamelogic.Position(5)] == core.services.gamelogic.Piece(
            type=core.services.gamelogic.PieceType.KING,
            color=core.services.gamelogic.Color.WHITE,
        )
        assert state[core.services.gamelogic.Position(60)] == core.services.gamelogic.Piece(
            type=core.services.gamelogic.PieceType.QUEEN,
            color=core.services.gamelogic.Color.BLACK,
        )
        assert state[core.services.gamelogic.Position(61)] == core.services.gamelogic.Piece(
            type=core.services.gamelogic.PieceType.KING,
            color=core.services.gamelogic.Color.BLACK,
        )
        assert state[core.services.gamelogic.Position(64)] == core.services.gamelogic.Piece(
            type=core.services.gamelogic.PieceType.ROOK,
            color=core.services.gamelogic.Color.BLACK,
        )


class BoardToStringboard(TestCase):

    def test_service(self):
        stringboard = """
        RKBQXBKR
        PPPPPPPP
        ........
        ........
        ........
        ........
        pppppppp
        rkbqxbkr
        """
        board = core.services.game.stringboard_to_board(stringboard)
        assert core.services.game.board_to_stringboard(board) == stringboard.strip().replace(' ', '')
