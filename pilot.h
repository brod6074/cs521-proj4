#ifndef PILOT_H
#define PILOT_H

#include <stack>
#include <libplayerc++/playerc++.h>

// Class: Pilot
// Implementation for the Pilot module.
class Pilot {
public:
	Pilot(std::stack<player_pose2d_t> waypoints): m_Waypoints(waypoints) {}
	player_pose2d_t getNextWaypoint();
	bool hasWaypoints() { return !m_Waypoints.empty(); }

private:
	std::stack<player_pose2d_t> m_Waypoints;
};

#endif