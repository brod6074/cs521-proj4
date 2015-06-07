// Programmer:	Roberto Rodriguez
// Class:		CS 521 - Robotics
// Project:		4 - Path Planning
// Due Date:	6/6/2015
// File: 		act.cpp
// Purpose: 	Implementation of Act class

#include "act.h"
#include "pilot.h"
#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

// Method: Act constructor. Sets dummy values
Act::Act() {
	goal.px = -1;
	goal.py = -1;
}

// Method: goalReached
// Calculates the distance between robot and the current goal and
// returns a bool indicating if the goal has been reached.
bool Act::goalReached(const PlayerCc::Position2dProxy& pp) {
	float GOAL_DISTANCE = 0.15;
	float dx = goal.px - pp.GetXPos();
	float dy = goal.py - pp.GetYPos();
	float dist = sqrt(dx * dx + dy * dy);
	return dist < GOAL_DISTANCE;
}

// Method: setGoal
// Sets a new waypoint/goal for the robot.
void Act::setGoal(player_pose2d_t wp) {
	goal = wp;
}

// Method: move
// Moves the robot to the nexy waypoint using potential field vectors for obstacle avoidance.
void Act::move(Position2dProxy &pp, LaserProxy &lp) {
	const double REPEL_FIELD_MINIMUM = 0.25;

	// Get potential field vectors
	FieldVector goalVector = getGoalVector(pp, goal);
	FieldVector avoidVector = avoidObstacle(pp, lp);

	// Determine whether the vector moves directly to the goal or must be repelled a bit

	pp.GoTo(goal);

	// NOTE: GoTo behavior from project 2 was not working correctly with this project so I
	// used the GoTo method from the Position2dProxy class.
	// if (goalVector + avoidVector < REPEL_FIELD_MINIMUM) {
	// 	player_pose2d_t repCoord = vecToRect(pp, avoidVector);
	// 	goal.px = repCoord.px;
	// 	goal.py = repCoord.py;
	// 	pp.GoTo(goal);
	// 	sleep(5);	
	// } else {
	// 	pp.GoTo(goal);
	// }
}

// Method: avoidObstacle
// Returns a potential field vector from a repulsion field. The LaserProxy
// is used to determine if an object is close enough to the robot to generate
// a non-trivial repulsion vector.
FieldVector Act::avoidObstacle(const Position2dProxy &pp, const LaserProxy &lp) {
	const double MAX_LASER_RANGE = 8.0;

	// Get sensor data from laser	
	double minR = lp.GetMinRight();
	double minL = lp.GetMinLeft();
	double min;
	double offset;

	// Determine which sensor detects the closer object. An offset of +/- 45
	// degrees will be added to the current yaw of the robot depending on the
	// choice of closest object. 
	if (minR < minL) {
		offset = 45;
		min = minR;
	} else {
		offset = -45;
		min = minL;
	}
		
	// If an object is in range, the magnitude of the repulsion vector will be dependent
	// on the distance between the object and the robot. The angle offset will be added to
	// the robot's yaw to determine the repulsion direction.
	//
	// Otherwise no object is in range and the repulsion vector's values are 0.
	FieldVector vec;
	if (min < MAX_LASER_RANGE) {
		vec.magnitude = MAX_LASER_RANGE - min; // Becomes larger as the robot moves closer
		vec.direction = normalize(pp.GetYaw() + dtor(offset));
	} else {
		vec.magnitude = 0;
		vec.direction = 0;
	}
	
	return vec;
}

// Method: getGoalVector
// Returns a potential field vector that always points in the direction of the goal
// relative to the robot.
FieldVector Act::getGoalVector(const Position2dProxy &pp, const player_pose2d_t &goal) {
	const double MAX_LASER_RANGE = 8.0;

	double dx = goal.px - pp.GetXPos();
	double dy = goal.py - pp.GetYPos();
 
	FieldVector vec;
	vec.direction = atan2(dy, dx);
	vec.magnitude = MAX_LASER_RANGE;

	return vec;
}

// Method: vecToRect
// Converts a field vector object to rectangular coordinates that can be used
// in the 2d grid.
player_pose2d_t Act::vecToRect(const Position2dProxy &pp, const FieldVector &v) {
	player_pose2d_t pos;

	pos.px = v.magnitude * cos(v.direction) + pp.GetXPos();
	pos.py = v.magnitude * sin(v.direction) + pp.GetYPos();

	return pos;
}

// Sums the magnitude of the vectors together
double operator+(const FieldVector &gotoVector, const FieldVector &repelVector) {
	return gotoVector.magnitude - repelVector.magnitude;
}