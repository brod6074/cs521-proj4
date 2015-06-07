// Programmer:	Roberto Rodriguez
// Class:	CS 521 - Robotics
// Project:	4 - Path Planning
// Due Date:	6/6/2015
// File: proj4.cc
// Purpose: Client program for Project 4

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
	LaserProxy lp(&robot, 0);

	robot.Read();
	player_pose2d_t start;
	player_pose2d_t goal;
	start.px = pp.GetXPos();
	start.py = pp.GetYPos();

	cout << "Please input the x-coordinate for the goal: ";
	cin >> goal.px;
	cout << "Please input the y-coordinate for the goal: ";
	cin >> goal.py;

	// Exit program if a path can't be generated with the given goal
	Navigator navigator;
	if (!navigator.setGoal(start, goal)) {
		cerr << "ERROR: Unreachable goal" << endl;
		cerr << "A valid path could not be generated" << endl;
		exit(EXIT_FAILURE);
	}

	// Get the set of waypoints from the navigator
	Pilot pilot(navigator.getWaypoints());
	Act act;

	// Keep passing waypoints from the pilot to the act until the final waypoint is reached
	while (pilot.hasWaypoints()) {
		player_pose2d_t nextWaypoint = pilot.getNextWaypoint();
		act.setGoal(nextWaypoint);
		while (!act.goalReached(pp)) {
			robot.Read();
			act.move(pp, lp);
		}
	}

	cout << "Final goal reached!" << endl;
	return 0;
}
