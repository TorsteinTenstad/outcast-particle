#pragma once

#include "components/area.hpp"
#include "components/physics.hpp"

bool CheckIntersection(Position position_a, Position position_b, Radius radius_a, Radius radius_b);
bool CheckIntersection(Position circle_position, Position square_position, Radius radius, WidthAndHeight width_and_height);