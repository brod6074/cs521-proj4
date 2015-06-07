#include "act.h"
#include "pilot.h"
#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

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
