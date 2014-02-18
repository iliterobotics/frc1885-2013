#include "CoordinateTransform.h"

#include <iostream>

using namespace std;

int main() 
{
	double thetaX, thetaY;

	CoordinateTransform::determineAngle(-3, 3, 5, thetaX, thetaY);

	cout<<"Theta X: "<<thetaX<<"\nTheta Y: "<<thetaY<<"\n";
}

