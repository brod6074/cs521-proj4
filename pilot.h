#ifndef PILOT_H
#define PILOT_H

#include <libplayerc++/playerc++.h>

// Class: Pilot
// Implementation for the Pilot module.
class Pilot {
public:
	Pilot();
	bool goalReached(const PlayerCc::Position2dProxy& pp);
	void setGoal(player_pose2d_t wp);
	void getDirection(const PlayerCc::Position2dProxy& pp, float& fSpeed, float& tSpeed);

private:
	player_pose2d_t goal;
};

#endif