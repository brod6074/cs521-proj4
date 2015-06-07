#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <stack>
#include <libplayerc++/playerc++.h>


struct Coordinate {
	Coordinate(int x, int y) : xLoc(x), yLoc(y) {}

	int xLoc;
	int yLoc;
};

class Navigator {
public:
	Navigator();
	player_pose2d_t nextWaypoint();
	bool hasWaypoints() { return !m_Waypoints.empty(); } 	
	bool setGoal(player_pose2d_t start, player_pose2d_t goal);

private:
	static const int m_GRID_ROWS = 250;
	static const int m_GRID_COLS = 600;
	static const int m_SCALE = 2;

	int m_Width;
	int m_Height;
	int m_MaxVal;
	float m_gridMap[m_GRID_ROWS][m_GRID_COLS];
	bool m_gridMarker[m_GRID_ROWS][m_GRID_COLS];
	bool m_gridVisited[m_GRID_ROWS][m_GRID_COLS];

	// Start/goal position represented with Coordinate type
	Coordinate m_StartCoord;
	Coordinate m_GoalCoord;

	std::stack<player_pose2d_t> m_Waypoints;

	void inputMap(bool print);
	void expandMap(int width, int height);
	void outputMap();
	bool createPlan();
	bool propagateWave();
	void extractPath();
	// void smoothPath();
	void printToText();
	Coordinate playerPoseToCoordinate(player_pose2d_t pp2d);
	player_pose2d_t coordinateToPlayerPose(Coordinate coord);
};

#endif
