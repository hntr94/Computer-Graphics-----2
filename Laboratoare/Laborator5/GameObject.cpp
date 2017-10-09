#include <iostream>
#include "GameObject.h"

using namespace std;

GameObject::GameObject()
{
	x = y = z = 0.0f;
	scaleX = scaleY = scaleZ = 0.0f;
	rotX = rotY = rotZ = 0.0f;
}

GameObject::~GameObject()
{

}

glm::mat4 GameObject::move(float posx, float posy, float posz)
{
	x = posx;
	y = posy;
	z = posz;
	return Transform3D::Translate(x, y, z);
}

glm::mat4 GameObject::scale(float eX, float eY, float eZ)
{
	scaleX = eX;
	scaleY = eY;
	scaleZ = eZ;
	return Transform3D::Scale(scaleX, scaleY, scaleZ);
}

glm::mat4 GameObject::rotx(float angle)
{
	rotX = angle;
	return Transform3D::RotateOX(rotX);
}

glm::mat4 GameObject::roty(float angle)
{
	rotY = angle;
	return Transform3D::RotateOY(rotY);
}

glm::mat4 GameObject::rotz(float angle)
{
	rotZ = angle;
	return Transform3D::RotateOZ(rotZ);
}

