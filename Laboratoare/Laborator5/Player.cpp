#include <iostream>
#include "Player.h"

Player::Player()
{
	x = 0;
	y = 0;
	z = 0;
	lives = 3;
	gunmode = 1;
}

Player::~Player()
{
}

int Player::getGunmode()
{
	return gunmode;
}

void Player::setGunmode(int g) 
{
	gunmode = g;
}
