#include "Laborator5.h"
#include "Transform3D.h"
#include "GameObject.h"
#include "Player.h"
#include "Tower.h"
#include "Lives.h"
#include "Bullet.h"
#include "Enemy.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

class Manager {
public:
	int ascendent_lives = 1;
	GameObject* player = new Player();
	float angle = RADIANS(180);
	std::vector<Tower*> towers;
	std::vector<Life*> lives;
	std::vector<Bullet*> bullets;
	std::vector<Bullet*> tower_bullets;
	std::vector<Enemy*> enemies;


private:


};