#include "GameObject.h"
#include "Transform3D.h"

class Enemy : public GameObject {
public:
	Enemy();
	Enemy(float px, float py, float pz, int lives);
	~Enemy();

	int lives;
	int segment;
	bool dead;
	float xvelocity, zvelocity;
	void Render();

private:
};