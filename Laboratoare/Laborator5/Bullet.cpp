#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

Bullet::Bullet(float scaleFactor, float px, float py, float pz, float angle, float sp)
{
	scaleX = scaleY = scaleZ = scaleFactor;
	x = px;
	y = py;
	z = pz;
	rotY = angle;
	speed = sp;
	damage = 1;
}

Bullet::Bullet(float scaleFactor, float px, float py, float pz, float angle, float sp, int dmg)
{
	scaleX = scaleY = scaleZ = scaleFactor;
	x = px;
	y = py;
	z = pz;
	rotY = angle;
	speed = sp;
	ySpeed = 1;
	damage = dmg;
	peak = false;
}

