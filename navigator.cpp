#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>

#include <libplayerc++/playerc++.h>

#include "navigator.h"

using namespace std;
using namespace PlayerCc;

// Method: NavModule
// Constructor for NavModule class.
// Accepts a pair of doubles to be used as goal coordinates.
Navigator::Navigator(Position2dProxy &pp) : m_StartCoord(-1, -1), m_GoalCoord(-1, -1) {
	m_Goal.px = 0; m_Goal.py = 0; m_Goal.pa = 0;

	// Initialize grid
	for (int i = 0; i < m_GRID_ROWS; i++) {
		for (int j = 0; j < m_GRID_COLS; j++) {
			m_gridMap[i][j] = 0.0;
			m_gridMarker[i][j] = false;
			m_gridVisited[i][j] = false;
		}
	}
	inputMap(true);
}

bool Navigator::setGoal(player_pose2d_t start, player_pose2d_t goal) {

	// code for convervsion of player_pose2d_t values to Coordinate values here
	// the member Coordinate values will be used

	m_StartCoord.xLoc = 120;
	m_StartCoord.yLoc = 100;
	m_GoalCoord.xLoc = 20;
	m_GoalCoord.yLoc = 20;

	return createPlan();
}

bool Navigator::createPlan() {
	if (!propagateWave()) {
		return false;
	}

	extractPath();
	//smoothPath();
	return true;
}

bool Navigator::propagateWave() {
	// convert to valid grid coordinates

	queue<Coordinate> coordinates;
	m_gridMap[m_GoalCoord.xLoc][m_GoalCoord.yLoc] = 2;
	coordinates.push(m_GoalCoord);

	while (!coordinates.empty()) {
		Coordinate curr = coordinates.front();
		coordinates.pop();
		int x = curr.xLoc;
		int y = curr.yLoc;

		if (x == m_StartCoord.xLoc && y == m_StartCoord.yLoc) {
			cout << "goal reached" << endl;
			//printToText();
			return true;
		}

		// check if current coordinate is against far left wall
		if (x != 0) {
			if (m_gridMap[x - 1][y] == 0) {
				m_gridMap[x - 1][y] = m_gridMap[x][y] + 1;
				Coordinate c3(x - 1, y);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is against far right wall
		if (x != m_GRID_ROWS - 1) {
			if (m_gridMap[x + 1][y] == 0) {
				m_gridMap[x + 1][y] = m_gridMap[x][y] + 1;
				Coordinate c3(x + 1, y);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is against top wall
		if (y != 0) {
			if (m_gridMap[x][y - 1] == 0) {
				m_gridMap[x][y - 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x, y - 1);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is against bottom wall
		if (y != m_GRID_COLS - 1) {
			if (m_gridMap[x][y + 1] == 0) {
				m_gridMap[x][y + 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x, y + 1);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is in top left corner
		if (x != 0 && y != 0) {
			if (m_gridMap[x - 1][y - 1] == 0) {
				m_gridMap[x - 1][y - 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x - 1, y - 1);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is in top right corner
		if (x != m_GRID_ROWS - 1 && y != 0) {
			if (m_gridMap[x + 1][y - 1] == 0) {
				m_gridMap[x + 1][y - 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x + 1, y - 1);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is in bottom left corner
		if (x != 0 && y != m_GRID_COLS - 1) {
			if (m_gridMap[x - 1][y + 1] == 0) {
				m_gridMap[x - 1][y + 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x - 1, y + 1);
				coordinates.push(c3);
			}
		}

		// check if current coordinate is in bottom right corner
		if (x != m_GRID_ROWS - 1 && y != m_GRID_COLS - 1) {
			if (m_gridMap[x + 1][y + 1] == 0) {
				m_gridMap[x + 1][y + 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x + 1, y + 1);
				coordinates.push(c3);
			}
		}
	}
	//printToText();
	return false;
}

void Navigator::extractPath() {
	stack<Coordinate> stack;
	stack.push(m_StartCoord);
	cout << "value at start:" << m_gridMap[m_StartCoord.xLoc][m_StartCoord.yLoc] << endl;

	while (true) {
		cout << "looping" << endl;
		Coordinate curr = stack.top();
		int x = curr.xLoc;
		int y = curr.yLoc;

		// all coordinates between start and goal are on the stack, so stop
		if (m_gridMap[x][y] == 2) {
			cout << "Path discovered" << endl;
			break;
		}

		if ((x != 0) && (m_gridMap[x-1][y] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x-1, y);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((x != m_GRID_ROWS - 1) && (m_gridMap[x+1][y] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x+1, y);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((y != 0) && (m_gridMap[x][y-1] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x, y-1);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((y != m_GRID_COLS - 1) && (m_gridMap[x][y+1] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x, y+1);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((x != 0 && y != 0) && (m_gridMap[x-1][y-1] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x-1, y-1);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((x != m_GRID_ROWS - 1 && y != 0) && (m_gridMap[x+1][y-1] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x+1, y-1);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((x != 0 && y != m_GRID_COLS - 1) && (m_gridMap[x-1][y+1] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x-1, y+1);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
		else if ((x != m_GRID_ROWS - 1 && y != m_GRID_COLS- 1) && (m_gridMap[x+1][y+1] == m_gridMap[x][y] - 1)) {
			Coordinate c3(x+1, y+1);
			cout << "coordinate pushed" << endl;
			stack.push(c3);
		}
	}

	cout << "Coordinates from start: " << m_StartCoord.xLoc << "," << m_StartCoord.yLoc;
	cout << "  to goal:" << m_GoalCoord.xLoc << "," << m_GoalCoord.yLoc << endl;
	cout << "stack contents:" << stack.size() << endl;
	while (!stack.empty()) {
		Coordinate c = stack.top();
		stack.pop();
		m_gridMap[c.xLoc][c.yLoc] = -7; // for debugging
		cout << "(" << c.xLoc << "," << c.yLoc << ")" << endl;
	}
	printToText();
}

// smoothPath definition here

// Method: nextWaypoint
// Returns the next waypoint for the pilot module
player_pose2d_t Navigator::nextWaypoint() {
	player_pose2d_t wp = m_Waypoints.front();
	m_Waypoints.pop();
	return wp;
}

void Navigator::inputMap(bool print) {
	char inputLine1[80], nextChar;
	int width, height, maxVal;

	ifstream inFile("hospital_section.pnm");
	inFile.getline(inputLine1,80);

	inFile >> width >> height >> maxVal;
	cout << "Width = " << width << ", Height = " << height << endl;

	m_Width = width / m_SCALE;
	m_Height = height / m_SCALE;
	m_MaxVal = maxVal;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			inFile >> nextChar;
			if (!nextChar) {  
				m_gridMap[i/m_SCALE][j/m_SCALE] = 1.0;
				m_gridMarker[i/m_SCALE][j/m_SCALE] = true;
			}
		}
	}

	expandMap(width, height);

	if (print) {
		outputMap();
	} 
}

void Navigator::expandMap(int width, int height) {
	for (int i = 1; i < m_GRID_ROWS; i++) {
		for (int j = 1; j < m_GRID_COLS; j++) {
			if (m_gridMarker[i][j] == true) {
				m_gridMap[i-1][j] = 1.0;
				m_gridMap[i+1][j] = 1.0;
				m_gridMap[i][j+1] = 1.0;
				m_gridMap[i][j-1] = 1.0;
			}
		}
	}
}

void Navigator::outputMap() {
	ofstream outFile("scaled_hospital_section.pnm");

	outFile << "P5" << endl;
	outFile << m_Width << " " << m_Height << endl
	    << m_MaxVal << endl;

	for (int i = 0; i < m_Height; i++) {
		for (int j = 0; j < m_Width; j++) {
			if (m_gridMap[i][j] == 1.0) {
				outFile << (char) 0;
			} else {
				outFile << (char) -1;
			}
		}
	}

	cout << "Scaled map output to file.\n";
}

void Navigator::printToText() {
	for (int i = 0; i < m_GRID_ROWS; i++) {
		for (int j = 0; j < m_GRID_COLS; j++) {
			cout << setw(4) << m_gridMap[i][j];
		}
		cout << endl;
	}
}

const int Navigator::m_GRID_ROWS;
const int Navigator::m_GRID_COLS;
const int Navigator::m_SCALE;
