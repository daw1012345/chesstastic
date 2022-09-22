from django.test import TestCase

import core.services.gamelogic

class AlgebraicToPosition(TestCase):

    def test_cases(self):
        cases = [
            ('A3', 17),
            ('F4', 30),
            ('A1', 1),
            ('H8', 64),
        ]
        for algebraic, expected_position in cases:
            assert core.services.gamelogic.algebraic_to_position(
                core.services.gamelogic.Algebraic(algebraic)
            ) == core.services.gamelogic.Position(expected_position)

    def test_upper_lower_case(self):
        lower = core.services.gamelogic.algebraic_to_position(
            core.services.gamelogic.Algebraic('a3')
        )
        upper = core.services.gamelogic.algebraic_to_position(
            core.services.gamelogic.Algebraic('A3')
        )
        assert lower == upper


class PositionToAlgebraic(TestCase):

    def test_cases(self):
        cases = [
            (17, 'A3'),
            (30, 'F4'),
            (1, 'A1'),
            (56, 'H7'),
            (64, 'H8'),
        ]
        for position, expected_algebraic in cases:
            assert core.services.gamelogic.position_to_algebraic(
                core.services.gamelogic.Position(position)
            ) == core.services.gamelogic.Algebraic(expected_algebraic)


class PositionToCoordinates(TestCase):

    def test_col(self):
        cases = [
            (1, 1),
            (4, 4),
            (8, 8),
            (64, 8),
        ]
        for position, expected_col in cases:
            assert core.services.gamelogic.position_to_coordinates(
                core.services.gamelogic.Position(position)
            ).col == expected_col

    def test_row(self):
        cases = [
            (1, 1),
            (17, 3),
            (8, 1),
            (64, 8),
            (63, 8),
        ]
        for position, expected_row in cases:
            assert core.services.gamelogic.position_to_coordinates(
                core.services.gamelogic.Position(position)
            ).row == expected_row


class CoordinatesToPosition(TestCase):

    def test_cases(self):
        cases = [
            (core.services.gamelogic.Coordinate(row=1, col=1), 1),
            (core.services.gamelogic.Coordinate(row=1, col=3), 3),
            (core.services.gamelogic.Coordinate(row=2, col=2), 10),
            (core.services.gamelogic.Coordinate(row=8, col=8), 64),
        ]
        for coordinate, expected_position in cases:
            position = core.services.gamelogic.coordinates_to_position(coordinate)
            assert core.services.gamelogic.Position(position) == expected_position

class GetBareMoves(TestCase):

    def test_king(self):
        black = core.services.gamelogic.Color.BLACK

        cases = [
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.KING), 1), [2, 9, 10]),
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.KING), 2), [1, 3, 9 ,10 ,11]),
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.KING), 64), [63, 56, 55])
        ]
        
        for piecelocation, expected_bare_moves in cases:
            piece, location = piecelocation
            expected_bare_moves = [core.services.gamelogic.Position(p) for p in expected_bare_moves]
            result = core.services.gamelogic.get_bare_moves(piece, location) 
            self.assertCountEqual(expected_bare_moves, result)

    def test_rook(self):
        black = core.services.gamelogic.Color.BLACK

        cases = [
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.ROOK), 1), [2, 3, 4, 5, 6, 7, 8, 9, 17, 25, 33, 41, 49, 57]),
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.ROOK), 2), [1, 3, 4, 5, 6, 7, 8, 10, 18, 26, 34, 42, 50, 58]),
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.ROOK), 64), [63, 62, 61, 60, 59, 58, 57, 56, 48, 40, 32, 24, 16, 8]),
        ]

        for piecelocation, expected_bare_moves in cases:
            piece, location = piecelocation
            expected_bare_moves = [core.services.gamelogic.Position(p) for p in expected_bare_moves]
            result = core.services.gamelogic.get_bare_moves(piece, location) 
            self.assertCountEqual(expected_bare_moves, result)

    def test_bishop(self):
        black = core.services.gamelogic.Color.BLACK

        cases = [
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.BISHOP), 1), [10, 19, 28, 37, 46, 55, 64]),
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.BISHOP), 28), [1, 10, 19, 37, 46, 55, 64, 21, 14, 7, 35, 42, 49]),
        ]

        for piecelocation, expected_bare_moves in cases:
            piece, location = piecelocation
            expected_bare_moves = [core.services.gamelogic.Position(p) for p in expected_bare_moves]
            result = core.services.gamelogic.get_bare_moves(piece, location) 
            self.assertCountEqual(expected_bare_moves, result)

    # def test_knight(self):
    #     black = core.services.gamelogic.Color.BLACK
        
    #     cases = [
    #         ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.KNIGHT), 1), [11, 18]),
    #        ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.KNIGHT), 2), [19, 12, 17]),
    #     ]

    #     for piecelocation, expected_bare_moves in cases:
    #         piece, location = piecelocation
    #         expected_bare_moves = [core.services.gamelogic.Position(p) for p in expected_bare_moves]
    #         result = core.services.gamelogic.get_bare_moves(piece, location) 
    #         self.assertCountEqual(expected_bare_moves, result)


    def test_queen(self):
        black = core.services.gamelogic.Color.BLACK

        cases = [
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.QUEEN), 1), [2, 3, 4, 5, 6, 7, 8, 9, 17, 25, 33, 41, 49, 57, 10, 19, 28, 37, 46, 55, 64]),
            ((core.services.gamelogic.Piece(color = black, type = core.services.gamelogic.PieceType.QUEEN), 57), [58, 59, 60, 61, 62, 63, 64, 49, 41, 33, 25, 17, 9, 1, 50, 43, 36, 29, 22, 15, 8]),
        ]

        for piecelocation, expected_bare_moves in cases:
            piece, location = piecelocation
            expected_bare_moves = [core.services.gamelogic.Position(p) for p in expected_bare_moves]
            result = core.services.gamelogic.get_bare_moves(piece, location) 
            self.assertCountEqual(expected_bare_moves, result)


class CanCastle(TestCase):

    def test_regular(self):
        board = core.services.game.stringboard_to_board("""
        RKBQXBKR
        PPPPPPPP
        ........
        ........
        ........
        ........
        pppppppp
        r...xbkr
        """)

        assert core.services.gamelogic.can_castle(
            board,
            core.services.gamelogic.Color.BLACK,
        ) == (False, False)
        assert core.services.gamelogic.can_castle(
            board,
            core.services.gamelogic.Color.WHITE,
        ) == (True, False)
