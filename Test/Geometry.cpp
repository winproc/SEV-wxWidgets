
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "Geometry.h"

std::vector<wxPoint> ComputePointsOnCircle(int Radius, int MaxPoints, int Inverted) {
	std::vector<wxPoint> ListOfPoints = {};

	float AngleStep = 6.28319 / MaxPoints;

	// Create a point for each angle step
	for (int Factor = 0; (Factor * AngleStep) < 6.28319; Factor++) {

		wxPoint LocusPoint(0, Radius * copysign(1,Inverted));

		// Angle is negative to ensure a clockwise rotation
		float Angle = -Factor * AngleStep;

		// Perform Euclidean rotation
		float ResultX, ResultY;

		ResultX = LocusPoint.x * cosf(Angle) - LocusPoint.y * sinf(Angle);
		ResultY = LocusPoint.x * sinf(Angle) + LocusPoint.y * cosf(Angle);

		LocusPoint.x = ResultX;
		LocusPoint.y = ResultY;

		ListOfPoints.push_back(LocusPoint);

	}

	return ListOfPoints;
}