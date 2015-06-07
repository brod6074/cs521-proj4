// Programmer:	Roberto Rodriguez
// Class:		CS 521 - Robotics
// Project:		4 - Path Planning
// Due Date:	6/6/2015
// File: 		pilot.cpp
// Purpose: 	Implementation of Pilot class

#include "pilot.h"
#include <libplayerc++/playerc++.h>

using namespace std;
using namespace PlayerCc;


player_pose2d_t Pilot::getNextWaypoint() {
	player_pose2d_t next = m_Waypoints.top();
	m_Waypoints.pop();
	return next;
}
