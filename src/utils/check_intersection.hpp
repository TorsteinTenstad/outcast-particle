#include "components/position.hpp"
#include "components/size.hpp"

bool CheckIntersection(Position* position_a, Position* position_b, Radius* radius_a, Radius* radius_b);
bool CheckIntersection(Position* position_a, Position* position_b, WidthAndHeight* width_and_height_a, WidthAndHeight* width_and_height_b);
bool CheckIntersection(Position* circle_position, Position* square_position, Radius* radius, WidthAndHeight* width_and_height);
bool CheckIntersection(Position* square_position, Position* circle_position, WidthAndHeight* width_and_height, Radius* radius);