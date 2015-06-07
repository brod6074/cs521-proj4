// Programmer:	Roberto Rodriguez
// Class:	CS 521 - Robotics
// Project:	4 - Path Planning
// Due Date:	6//2015

#include <libplayerc++/playerc++.h>
#include <iostream>
#include "navigator.h"
#include "pilot.h"

using namespace std;
using namespace PlayerCc;

int main() {
	// Create all neccessary components
	PlayerClient robot("localhost");
	Position2dProxy pp(&robot, 0);
	Navigator navigator;
	Pilot pilot;

	robot.Read();
	player_pose2d_t start;
	start.px = pp.GetXPos();
	start.py = pp.GetYPos();
	player_pose2d_t goal;
	goal.px = -7.0;
	goal.py = 3.0;

	if (!navigator.setGoal(start, goal)) {
		cerr << "ERROR: Unreachable goal" << endl;
		cerr << "A valid path could not be generated" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Starting coordinate: (" << start.px << "," << start.py << ")\n";
	cout << "Goal coordinate: (" << goal.px << "," << goal.py << ")\n";


	while (navigator.hasWaypoints()) {
		player_pose2d_t next = navigator.nextWaypoint();
		pilot.setGoal(next);
		while (!pilot.goalReached(pp)) {
			robot.Read();
			pp.GoTo(next);
		}
	}

	return 0;
}
