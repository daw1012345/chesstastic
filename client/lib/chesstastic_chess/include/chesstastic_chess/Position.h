//
// Created by degra on 01/11/2021.
//

#ifndef CHESSTASTIC_POSITION_H
#define CHESSTASTIC_POSITION_H

class Position {
public:
    /** This function will create an instance of Position with an integer to represent the position.
     * @param position The integer (has te be 0 <= x <=63)
     */
    Position(int position);
    /** This function will create an instance of Position with an x and y coordinate.
     * @param x The x coordinate, horizontal
     * @param y The y coordinate, vertical
     */
    Position(int x, int y);

public:
    /** This function can be used to get the Position.
     * @return An integer (0 <= x <=63) to represent the location
     */
    int getPosition() const;
    /** This function can be used to set the position.
     * @param position An integer to represent the position (0 <= x <= 63)
     */
    void setPosition(int position);
    /** This function can be used to retrieve the x coordinate of the Position.
     * @return the x coordinate of the function, horizontal coordinate (0 <= x <= 7)
     */
    int getXCoordinate();
    /** This function can be used to retrieve the y coordinate of the Position.
     * @return the y coordinate of the function, vertical coordinate (0 <= y <= 7)
     */
    int getYCoordinate();
    /** This function can be used to set the x and y coordinate of the Position.
     * @param x The x coordinate, horizontal
     * @param y The y coordinate, vertical
     */
    void setCoordinates(int x, int y);

    bool operator==(Position position2) const;
    bool operator!=(Position position2) const;
    bool operator<(Position position2) const;
    bool operator>(Position position2) const;

private:
    int position;
};
#endif //CHESSTASTIC_POSITION_H
