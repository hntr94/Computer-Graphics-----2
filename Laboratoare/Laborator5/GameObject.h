#pragma once

#include "Laborator5.h"
#include "Transform3D.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

class GameObject {
public:
	float x, y, z;
	float scaleX, scaleY, scaleZ;
	float rotX, rotY, rotZ;
	
	GameObject();
	~GameObject();
	glm::mat4 move(float posx, float posy, float posz);
	glm::mat4 scale(float scaleX, float scaleY, float scaleZ);
	glm::mat4 GameObject::rotx(float angle);
	glm::mat4 GameObject::roty(float angle);
	glm::mat4 GameObject::rotz(float angle);

private:
	


};