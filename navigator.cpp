#include <iostream>
#include <fstream>
#include <libplayerc++/playerc++.h>
#include "navigator.h"

using namespace std;
using namespace PlayerCc;

// Method: NavModule
// Constructor for NavModule class.
// Accepts a pair of doubles to be used as goal coordinates.

Navigator::Navigator(double x, double y, Position2dProxy &pp) {
	m_Goal.px = x;
	m_Goal.py = y;
	for (int i = 0; i < m_GRID_SIZE; i++) {
		for (int j = 0; j < m_GRID_SIZE; j++) {
			m_gridMap[i][j] = 0.0;
			m_gridMarker[i][j] = false;
		}
	}
	inputMap(true);
	// createPlan()

}

void Navigator::createPlan() {
	
}

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

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			inFile >> nextChar;
			if (!nextChar) {  
				m_gridMap[i/m_SCALE][j/m_SCALE] = 1.0;
				m_gridMarker[i/m_SCALE][j/m_SCALE] = true;
			}
		}
	}

    cout << "Map input complete.\n";

	expandMap(width, height);

	if (print) {
		ofstream outFile("scaled_hospital_section.pnm");

		outFile << inputLine1 << endl;
		outFile << width/m_SCALE << " " << height/m_SCALE << endl
	      << maxVal << endl;

		for (int i = 0; i < height/m_SCALE; i++) {
			for (int j = 0; j < width/m_SCALE; j++) {
				if (m_gridMap[i][j] == 1.0) {
					outFile << (char) 0;
				} else {
					outFile << (char) -1;
				}
			}
		}

		cout << "Scaled map output to file.\n";
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

const int Navigator::m_GRID_SIZE;
const int Navigator::m_SCALE;
