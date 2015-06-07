// Programmer:	Roberto Rodriguez
// Class:		CS 521 - Robotics
// Project:		4 - Path Planning
// Due Date:	6/6/2015
// File: 		act.h
// Purpose: 	Specification of Act class

#ifndef ACT_H
#define ACT_H

#include <libplayerc++/playerc++.h>

// Represents a potential field vector
struct FieldVector {
	double direction;
	double magnitude;
};

// Act module
class Act {
public:
	Act();
	bool goalReached(const PlayerCc::Position2dProxy& pp);
	void setGoal(player_pose2d_t wp);
	void move(PlayerCc::Position2dProxy &pp, PlayerCc::LaserProxy &lp);

private:
	player_pose2d_t goal;

	FieldVector avoidObstacle(const PlayerCc::Position2dProxy &pp, const PlayerCc::LaserProxy &lp);
	FieldVector getGoalVector(const PlayerCc::Position2dProxy &pp, const player_pose2d_t &goal);
	player_pose2d_t vecToRect(const PlayerCc::Position2dProxy &pp, const FieldVector &v);
	friend double operator+(const FieldVector &gotoVector, const FieldVector &repelVector);
};

double operator+(const FieldVector &gotoVector, const FieldVector &repelVector);

#endif
