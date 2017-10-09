#include "GameObject.h"
#include "Transform3D.h"

class Bullet : public GameObject {
public:
	int type;
	float speed;
	int damage;
	float ySpeed;
	bool peak;
	float time;
	Bullet();
	~Bullet();
	Bullet(float scaleFactor, float px, float py, float pz, float angle, float speed);
	Bullet(float scaleFactor, float px, float py, float pz, float angle, float speed, int damage);

private:

	

};