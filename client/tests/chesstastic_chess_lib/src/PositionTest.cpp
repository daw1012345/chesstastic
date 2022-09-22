//
// Created by degra on 04/11/2021.
//

#include <gtest/gtest.h>
#include <chesstastic_chess/Position.h>

// Test for getPosition(), check if it returns correct value.
TEST(PositionTest, GetPosition) {
    auto pos1 = Position(1);
    auto pos2 = Position(2);
    auto pos3 = Position(46);
    EXPECT_EQ(1, pos1.getPosition());
    EXPECT_EQ(2, pos2.getPosition());
    EXPECT_EQ(46, pos3.getPosition());
}

// Test for setPosition(int), check if correct position is set.
TEST(PositionTest, SetPosition) {
    auto pos = Position(1);
    EXPECT_EQ(1, pos.getPosition());
    pos.setPosition(2);
    EXPECT_EQ(2, pos.getPosition());
}

// Test for getYCoordinate(), check if the correct y coordinate is returned for a number of positions.
TEST(PositionTest, GetYCoordinate) {
    auto pos = Position(0);
    EXPECT_EQ(0, pos.getYCoordinate());
    pos.setPosition(7);
    EXPECT_EQ(0, pos.getYCoordinate());
    pos.setPosition(8);
    EXPECT_EQ(1, pos.getYCoordinate());
    pos.setPosition(15);
    EXPECT_EQ(1, pos.getYCoordinate());
    pos.setPosition(16);
    EXPECT_EQ(2, pos.getYCoordinate());
    pos.setPosition(23);
    EXPECT_EQ(2, pos.getYCoordinate());
    pos.setPosition(24);
    EXPECT_EQ(3, pos.getYCoordinate());
    pos.setPosition(31);
    EXPECT_EQ(3, pos.getYCoordinate());
    pos.setPosition(32);
    EXPECT_EQ(4, pos.getYCoordinate());
    pos.setPosition(39);
    EXPECT_EQ(4, pos.getYCoordinate());
    pos.setPosition(40);
    EXPECT_EQ(5, pos.getYCoordinate());
    pos.setPosition(47);
    EXPECT_EQ(5, pos.getYCoordinate());
    pos.setPosition(48);
    EXPECT_EQ(6, pos.getYCoordinate());
    pos.setPosition(55);
    EXPECT_EQ(6, pos.getYCoordinate());
    pos.setPosition(56);
    EXPECT_EQ(7, pos.getYCoordinate());
    pos.setPosition(63);
    EXPECT_EQ(7, pos.getYCoordinate());
}
// Test for getXCoordinate(), check if the correct x coordinate is returned for a number of positions.
TEST(PositionTest, GetXCoordinate) {
    auto pos = Position(0);
    EXPECT_EQ(0, pos.getXCoordinate());
    pos.setPosition(7);
    EXPECT_EQ(7, pos.getXCoordinate());
    pos.setPosition(8);
    EXPECT_EQ(0, pos.getXCoordinate());
    pos.setPosition(15);
    EXPECT_EQ(7, pos.getXCoordinate());
    pos.setPosition(19);
    EXPECT_EQ(3, pos.getXCoordinate());
    pos.setPosition(21);
    EXPECT_EQ(5, pos.getXCoordinate());
    pos.setPosition(60);
    EXPECT_EQ(4, pos.getXCoordinate());
    pos.setPosition(63);
    EXPECT_EQ(7, pos.getXCoordinate());
}

// Test for setCoordinates(int, int), check if correct position is set for a number of valid coordinates.
TEST(PositionTest, SetValidCoordinates) {
    auto pos = Position(0);
    pos.setCoordinates(4,2);
    EXPECT_EQ(4, pos.getXCoordinate());
    EXPECT_EQ(2, pos.getYCoordinate());
    EXPECT_EQ(20, pos.getPosition());
    pos.setCoordinates(7,7);
    EXPECT_EQ(63, pos.getPosition());
    pos.setCoordinates(0,0);
    EXPECT_EQ(0,pos.getPosition());
}

// Test for setCoordinates(int, int), check that invalid position is set for a number of invalid coordinates.
TEST(PositionTest, SetInvalidCoordinates) {
    auto pos = Position(0);
    pos.setCoordinates(-1, -1);
    EXPECT_FALSE(pos.getPosition() >= 0 && pos.getPosition() <= 63);
    pos.setCoordinates(8, 1);
    EXPECT_FALSE(pos.getPosition() >= 0 && pos.getPosition() <= 63);
    pos.setCoordinates(1, 8);
    EXPECT_FALSE(pos.getPosition() >= 0 && pos.getPosition() <= 63);
    pos.setCoordinates(-5, 5);
    EXPECT_FALSE(pos.getPosition() >= 0 && pos.getPosition() <= 63);
}

// Test for operator==(Position), check if two instances of Position are considered equal when their positions are equal.
TEST(PositionTest, OperatorEquals) {
    auto pos1 = Position(1);
    auto pos2 = Position(2);
    auto pos3 = Position(1);
    EXPECT_TRUE(pos1 == pos3);
    EXPECT_FALSE(pos2 == pos3);
    pos3.setPosition(2);
    EXPECT_TRUE(pos2 == pos3);
}

// Test for operator<(Position), check if an instance of Position is considered less than another instance of Position if their position is less.
TEST(PositionTest, OperatorLess) {
    auto pos1 = Position(1);
    auto pos2 = Position(2);
    auto pos3 = Position(1);
    EXPECT_TRUE(pos1 < pos2);
    EXPECT_FALSE(pos2 < pos1);
    EXPECT_FALSE(pos1 < pos3);
    EXPECT_FALSE(pos3 < pos1);
}

// Test for operator>(Position), check if an instance of Position is considered greater than another instance of Position if their position is greater.
TEST(PositionTest, OperatorGreater) {
    Position pos1 = Position(1);
    Position pos2 = Position(2);
    Position pos3 = Position(1);
    EXPECT_FALSE(pos1 > pos2);
    EXPECT_TRUE(pos2 > pos1);
    EXPECT_FALSE(pos1 > pos3);
    EXPECT_FALSE(pos3 > pos1);
}

// Test for operator!=(Position), check if an instance of Position is considered unequal to another instance of Position if the position values are not equal.
TEST(PositionTest, OperatorNotEqual) {
    Position pos1 = Position(1);
    Position pos2 = Position(2);
    Position pos3 = Position(1);
    EXPECT_TRUE(pos1 != pos2);
    EXPECT_TRUE(pos2 != pos1);
    EXPECT_FALSE(pos1 != pos3);
    EXPECT_FALSE(pos3 != pos1);
}
