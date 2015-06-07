#include "pilot.h"
#include <libplayerc++/playerc++.h>

using namespace std;
using namespace PlayerCc;


player_pose2d_t Pilot::getNextWaypoint() {
	player_pose2d_t next = m_Waypoints.top();
	m_Waypoints.pop();
	return next;
}

// Method: getDirection
// Sets the forward and turn speeds based on where the waypoint is relative to the
// look angle of the robot. 
// void Pilot::getDirection(const PlayerCc::Position2dProxy& pp, float& fSpeed, float& tSpeed) {
// 	const float MIN_DELTA_THETA = 0.090;
// 	float dx = goal.px - pp.GetXPos();
// 	float dy = goal.py - pp.GetYPos();
// 	float dist = sqrt(dx * dx + dy * dy);
// 	float robotGoalAngle = asin(dy / dist);
// 	float robotLookAngle = pp.GetYaw();
// 	float deltaTheta = robotGoalAngle - robotLookAngle;

// 	if (abs(deltaTheta) <= MIN_DELTA_THETA) {
// 		fSpeed = 20;
// 		tSpeed = 0;
// 	} else if (robotGoalAngle >= 0 && robotLookAngle >=0 && deltaTheta >= 0) {
// 		fSpeed = 0;
// 		tSpeed = 40;
// 	} else if (robotGoalAngle >= 0 && robotLookAngle >=0 && deltaTheta < 0) {
// 		fSpeed = 0;
// 		tSpeed = -40;
// 	} else if (robotGoalAngle < 0 && robotLookAngle < 0 && deltaTheta < 0) {
// 		fSpeed = 0;
// 		tSpeed = -40;
// 	} else {
// 		fSpeed = 0;
// 		tSpeed = 40;
// 	}
// }
