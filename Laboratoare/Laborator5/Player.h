#pragma once
#include "GameObject.h"
#include "Transform3D.h"

class Player : public GameObject {
	public:
		Player();
		~Player();
		int getGunmode();
		void setGunmode(int g);
		int gunmode;

	private:
		int lives;
		
};