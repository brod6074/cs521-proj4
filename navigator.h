#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <libplayerc++/playerc++.h>
#include <queue>

// Class: NavModule
// Implementation of the Navigator module. Contains a collection of waypoints
// to be sent to a Pilot module.

class Navigator {
public:
	Navigator(double x, double y, PlayerCc::Position2dProxy &pp);
	player_pose2d_t nextWaypoint();
	bool hasWaypoints() { return !m_Waypoints.empty(); } 	

private:
	static const int m_GRID_SIZE = 1000;
	static const int m_SCALE = 2;

	float m_gridMap[m_GRID_SIZE][m_GRID_SIZE];
	bool m_gridMarker[m_GRID_SIZE][m_GRID_SIZE];
	std::queue<player_pose2d_t> m_Waypoints;
	player_pose2d_t m_Goal;

	void inputMap(bool print);
	void expandMap(int width, int height);
	void createPlan();
};

#endif
