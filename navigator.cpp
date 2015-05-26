#include <iostream>
#include <fstream>
#include <libplayerc++/playerc++.h>
#include "navigator.h"

using namespace std;
using namespace PlayerCc;

// Method: NavModule
// Constructor for NavModule class.
// Accepts a pair of doubles to be used as goal coordinates.

Navigator::Navigator(Position2dProxy &pp) {
	m_Goal.px = 0; m_Goal.py = 0; m_Goal.pa = 0;

	for (int i = 0; i < m_GRID_SIZE; i++) {
		for (int j = 0; j < m_GRID_SIZE; j++) {
			m_gridMap[i][j] = 0.0;
			m_gridMarker[i][j] = false;
		}
	}
	inputMap(true);
}

bool Navigator::setGoal(double x, double y) {
	return createPlan(x, y);
}

bool Navigator::createPlan(double x, double y) {
	if (!propagateWave(x, y)) {
		return false;
	}
	//extractPath();
	//smoothPath();
	return true;
}

bool Navigator::propagateWave(double goalX, double goalY) {
	// convert to valid grid coordinates

	int x1 = 40;
	int y1 = 94;

	if (m_gridMap[x1][y1] == 1)
		cout << "START LOCATION IS 1" << endl;
	else
		cout << "START LOCATION IS 0" << endl;

	int x2 = 20;
	int y2 = 20;

	if (m_gridMap[x2][y2] == 1)
		cout << "END LOCATION IS 1" << endl;
	else
		cout << "END LOCATION IS 0" << endl;

	queue<Coordinate> coordinates;
	m_gridMap[x2][y2] = 2;
	Coordinate c1(x2, y2);
	coordinates.push(c1);

	while (!coordinates.empty()) {
		Coordinate c2 = coordinates.front();
		coordinates.pop();
		int x = c2.xLoc;
		int y = c2.yLoc;

		//cout << "x = " << x << " , y = " << y << endl;

		if (x != 0) {
			if ((m_gridMap[x - 1][y] == 0) && (x-1 == x1) && (y == y1)) {
				cout << "goal reached" << endl;
				return true;
			}

			if (m_gridMap[x - 1][y] == 0) {
				m_gridMap[x - 1][y] = m_gridMap[x][y] + 1;
				Coordinate c3(x - 1, y);
				coordinates.push(c3);
				cout << "pushing: " << c3.xLoc << " , " << c3.yLoc << endl;
			}
		}
		if (x != m_GRID_SIZE - 1) {
			if ((m_gridMap[x + 1][y] == 0) && (x+1 == x1) && (y == y1)) {
				cout << "goal reached" << endl;
				return true;
			}
			if (m_gridMap[x + 1][y] == 0) {
				m_gridMap[x + 1][y] = m_gridMap[x][y] + 1;
				Coordinate c3(x + 1, y);
				coordinates.push(c3);
				cout << "pushing: " << c3.xLoc << " , " << c3.yLoc << endl;
			}
		}
		if (y != 0) {
			if ((m_gridMap[x][y - 1] == 0) && (x == x1) && (y-1 == y1)) {
				cout << "goal reached" << endl;
				return true;
			}
			if (m_gridMap[x][y - 1] == 0) {
				m_gridMap[x][y - 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x, y - 1);
				coordinates.push(c3);
				cout << "pushing: " << c3.xLoc << " , " << c3.yLoc << endl;
			}
		}
		if (y != m_GRID_SIZE) {
			if ((m_gridMap[x][y + 1] == 0) && (x == x1) && (y+1 == y1)) {
				cout << "goal reached" << endl;
				return true;
			}
			if (m_gridMap[x][y + 1] == 0) {
				m_gridMap[x][y + 1] = m_gridMap[x][y] + 1;
				Coordinate c3(x, y + 1);
				coordinates.push(c3);
				cout << "pushing: " << c3.xLoc << " , " << c3.yLoc << endl;
			}
		}
	}
	printToText();
	return true;
}

// extractPath definition here

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
	for (int i = 1; i < m_GRID_SIZE; i++) {
		for (int j = 1; j < m_GRID_SIZE; j++) {
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
	ofstream outFile("output.txt");

	for (int i = 0; i < m_Height; i++) {
		for (int j = 0; j < m_Width; j++) {
			outFile << m_gridMap[i][j];
		}
		outFile << endl;
	}
}

const int Navigator::m_GRID_SIZE;
const int Navigator::m_SCALE;
