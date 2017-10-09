#include <iostream>
#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(float px, float py, float pz, int life)
{
	x = px;
	y = py;
	z = pz;
	segment = 1;
	xvelocity = 0.1;
	zvelocity = 0;
	rotY = RADIANS(90);
	lives = life;
	dead = false;
}

Enemy::~Enemy()
{
}

void Enemy::Render()
{
	//RenderMesh(meshes["tower"], shaders["Simple"], modelMatrix);
}
