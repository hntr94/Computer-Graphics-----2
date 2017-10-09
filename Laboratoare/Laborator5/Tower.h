#include "GameObject.h"
#include "Transform3D.h"


class Tower : public GameObject {
public:
	Tower();
	~Tower();
	Tower(float px, float py, float pz);


private:
	int lives;
	float angle;
};