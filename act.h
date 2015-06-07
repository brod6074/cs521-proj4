#ifndef ACT_H
#define ACT_H

#include <libplayerc++/playerc++.h>

class Act {
public:
	Act();
	bool goalReached(const PlayerCc::Position2dProxy& pp);
	void setGoal(player_pose2d_t wp);
private:
	player_pose2d_t goal;
};

#endif
