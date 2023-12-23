#pragma once

/*

Compute the locations of all points that will be used as the position for the engine toggle circles

@param Radius Radius of the ring


*/
std::vector<wxPoint> ComputePointsOnCircle(int Radius, int MaxPoints, int Inverted);
