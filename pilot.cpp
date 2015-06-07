#include "pilot.h"
#include <libplayerc++/playerc++.h>

using namespace std;
using namespace PlayerCc;

Pilot::Pilot() {
	goal.px = -1;
	goal.py = -1;
}

// Method: goalReached
// Calculates the distance between robot and the current goal and
// returns a bool indicating if the goal has been reached.
bool Pilot::goalReached(const PlayerCc::Position2dProxy& pp) {
	float GOAL_DISTANCE = 0.15;
	float dx = goal.px - pp.GetXPos();
	float dy = goal.py - pp.GetYPos();
	float dist = sqrt(dx * dx + dy * dy);
	return dist < GOAL_DISTANCE;
}

// Method: setGoal
// Sets a new waypoint/goal for the robot.
void Pilot::setGoal(player_pose2d_t wp) {
	goal = wp;
}

// Method: getDirection
// Sets the forward and turn speeds based on where the waypoint is relative to the
// look angle of the robot. 
void Pilot::getDirection(const PlayerCc::Position2dProxy& pp, float& fSpeed, float& tSpeed) {
	const float MIN_DELTA_THETA = 0.090;
	float dx = goal.px - pp.GetXPos();
	float dy = goal.py - pp.GetYPos();
	float dist = sqrt(dx * dx + dy * dy);
	float robotGoalAngle = asin(dy / dist);
	float robotLookAngle = pp.GetYaw();
	float deltaTheta = robotGoalAngle - robotLookAngle;

	if (abs(deltaTheta) <= MIN_DELTA_THETA) {
		fSpeed = 20;
		tSpeed = 0;
	} else if (robotGoalAngle >= 0 && robotLookAngle >=0 && deltaTheta >= 0) {
		fSpeed = 0;
		tSpeed = 40;
	} else if (robotGoalAngle >= 0 && robotLookAngle >=0 && deltaTheta < 0) {
		fSpeed = 0;
		tSpeed = -40;
	} else if (robotGoalAngle < 0 && robotLookAngle < 0 && deltaTheta < 0) {
		fSpeed = 0;
		tSpeed = -40;
	} else {
		fSpeed = 0;
		tSpeed = 40;
	}
}
