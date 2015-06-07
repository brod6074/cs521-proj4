// Programmer:	Roberto Rodriguez
// Class:	CS 521 - Robotics
// Project:	4 - Path Planning
// Due Date:	6//2015

#include <libplayerc++/playerc++.h>
#include <iostream>
#include "navigator.h"
#include "pilot.h"
#include "act.h"

using namespace std;
using namespace PlayerCc;

int main() {
	// Create all neccessary components
	PlayerClient robot("localhost");
	Position2dProxy pp(&robot, 0);
	Navigator navigator;

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

	Pilot pilot(navigator.getWaypoints());
	Act act;
	while (pilot.hasWaypoints()) {
		player_pose2d_t next = pilot.getNextWaypoint();
		act.setGoal(next);
		while (!act.goalReached(pp)) {
			robot.Read();
			pp.GoTo(next);
		}
	}
	
	cout << "Final goal reached!" << endl;
	return 0;
}
