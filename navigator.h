#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <libplayerc++/playerc++.h>
#include <queue>

// Class: NavModule
// Implementation of the Navigator module. Contains a collection of waypoints
// to be sent to a Pilot module.

struct Coordinate {
	Coordinate(int x, int y) : xLoc(x), yLoc(y) {}

	int xLoc;
	int yLoc;
};

class Navigator {
public:
	Navigator(PlayerCc::Position2dProxy &pp);
	player_pose2d_t nextWaypoint();
	bool hasWaypoints() { return !m_Waypoints.empty(); } 	
	bool setGoal(player_pose2d_t start, player_pose2d_t goal);

private:
	static const int m_GRID_SIZE = 800;
	static const int m_SCALE = 2;

	int m_Width;
	int m_Height;
	int m_MaxVal;
	float m_gridMap[m_GRID_SIZE][m_GRID_SIZE];
	bool m_gridMarker[m_GRID_SIZE][m_GRID_SIZE];

	Coordinate startCoord;
	Coordinate goalCoord;

	std::queue<player_pose2d_t> m_Waypoints;
	player_pose2d_t m_Goal;

	void inputMap(bool print);
	void expandMap(int width, int height);
	void outputMap();
	bool createPlan();
	bool propagateWave();
	void extractPath();
	// void smoothPath();
	void printToText();
};

#endif
