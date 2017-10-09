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

#define INF 123456789
#define PI 3.141592652
#define GRENADE 5
#define PISTOL 1
#define SHOTGUN 2

using namespace std;

glm::vec3 aux_position = glm::vec3(1);
glm::vec3 aux_up = glm::vec3(1);
glm::vec3 aux_right = glm::vec3(1);
glm::vec3 aux_forward = glm::vec3(1);
int bullet_mode = 1;
float bt = 1.0f;
float et = 5.0f;
float aux_angle = RADIANS(180);
int bullet_damage = 1;
bool from_build = false;
int ascendent_lives = 1;
Player* player = new Player();
float angle = RADIANS(180);
vector<Tower*> towers;
vector<Life*> lives;
vector<Bullet*> bullets;
vector<Bullet*> tower_bullets;
vector<Enemy*> enemies;

float enemy_time = Engine::GetElapsedTime();
float bullet_time = Engine::GetElapsedTime();
float player_bullet = Engine::GetElapsedTime();

float euclidiad_distance(float posx1, float posy1, float posx2, float posy2) {
	float x = posx1 - posx2; //calculating number to square in next step
	float y = posy1 - posy2;
	float dist;

	dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
	dist = sqrt(dist);

	return dist;
}

Laborator5::Laborator5()
{
}

Laborator5::~Laborator5()
{
}

void Laborator5::Init()
{
	renderCameraTarget = true;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));


	camera2 = new Laborator::Camera();
	camera2->Set(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));


	towers.push_back(new Tower(3, 0, 2));
	towers.push_back(new Tower(-3, 0, 1));
	towers.push_back(new Tower(-1, 0, -2));

	lives.push_back(new Life());
	lives.push_back(new Life());
	lives.push_back(new Life());

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("archer");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Archer", "Archer.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("tower");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Tower", "Only Tower.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("enemy");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters", "brawler_armoured.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	fov = 60.0;
	width = 20;
	height = 20;
	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	// TODO projection matrix tip ortho

	projectionMatrix2 = glm::ortho(-width, width, -height, height, 0.01f, 250.0f);

}

void Laborator5::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Laborator5::Update(float deltaTimeSeconds)
{
	int i;

	if (enemy_time + et < Engine::GetElapsedTime() && player->gunmode != 4) {
		if(et > 3) et -= 0.01;
		enemies.push_back(new Enemy(-15, 0, -15, ascendent_lives+=2));
		enemy_time = Engine::GetElapsedTime();
	}
	if (player->gunmode == 4) enemies.clear();

	// time for towers to shoot
	if (bullet_time + 2 < Engine::GetElapsedTime()) {
		if (!enemies.empty()) {
			for (i = 0; i < towers.size(); i++)
				tower_bullets.push_back(new Bullet(1.5f, towers[i]->x, towers[i]->y + 1, towers[i]->z, towers[i]->rotY, 0.5));
		}
		bullet_time = Engine::GetElapsedTime();
	}

	// delete useless bullets
	for (i = 0; i < tower_bullets.size(); i++) {
		if (tower_bullets[i]->x > 100 || tower_bullets[i]->z > 100 || tower_bullets[i]->x < -100 || tower_bullets[i]->z < -100) {
			tower_bullets.erase(tower_bullets.begin() + i);
			if (i > 0) i--;
		}
		
		glm::mat4 modelMatrix = glm::mat4(1);
		if (!tower_bullets.empty()) {
			tower_bullets[i]->x += cos(tower_bullets[i]->rotY) * tower_bullets[i]->speed;
			tower_bullets[i]->z += sin(tower_bullets[i]->rotY) * tower_bullets[i]->speed;
			modelMatrix *= tower_bullets[i]->move(tower_bullets[i]->x, 0, tower_bullets[i]->z);

			modelMatrix *= tower_bullets[i]->scale(tower_bullets[i]->scaleX, tower_bullets[i]->scaleY, tower_bullets[i]->scaleZ);
			RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
		}
	}

	for (i = 0; i < bullets.size(); i++) {
		if (bullets[i]->x > 100 || bullets[i]->z > 100 || bullets[i]->x < -100 || bullets[i]->z < -100) {
			bullets.erase(bullets.begin() + i);
			if (i > 0) i--;
		}
		if (!bullets.empty())
			if (bullets[i]->damage == 1 && euclidiad_distance(bullets[i]->x, bullets[i]->z, player->x, player->z) > 15) {
				bullets.erase(bullets.begin() + i);
				if (i > 0) i--;
			}

		glm::mat4 modelMatrix = glm::mat4(1);
		if (!bullets.empty()) {
			bullets[i]->x += cos(bullets[i]->rotY) * bullets[i]->speed;
			bullets[i]->z += sin(bullets[i]->rotY) * bullets[i]->speed;
			if (player->gunmode == 3 && bullets[i]->y >= 0) {
				if (bullets[i]->y <= 2.5f && !bullets[i]->peak)
				{	
					bullets[i]->y += bullets[i]->time;
					bullets[i]->time -= 0.003f;
				}
				else {
					bullets[i]->peak = true;
					bullets[i]->time = 0.02f;
				}

				if (bullets[i]->peak) {
					bullets[i]->y -= bullets[i]->time;
					bullets[i]->time -= 0.0005;
				}
				//cout << bullets[i]->y - (bullets[i]->ySpeed);
				//bullets[i]->ySpeed -= 0.01;
			}
			modelMatrix *= bullets[i]->move(bullets[i]->x, bullets[i]->y, bullets[i]->z);

			modelMatrix *= bullets[i]->scale(bullets[i]->scaleX, bullets[i]->scaleY, bullets[i]->scaleZ);
			RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
		}
	}

	// render towers
	for (i = 0; i < towers.size(); i++) {
		float euclidean = INF;
		for (int j = 0 ; j < enemies.size(); j++) {
			float aux = euclidiad_distance(towers[i]->x, towers[i]->z, enemies[j]->x, enemies[j]->z);
			if (euclidean > euclidiad_distance(towers[i]->x, towers[i]->z, enemies[j]->x, enemies[j]->z)) {
				euclidean = aux;
				towers[i]->rotY = atan2(enemies[j]->z - towers[i]->z, enemies[j]->x - towers[i]->x) ;
			}
		}

		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= towers[i]->move(towers[i]->x, towers[i]->y, towers[i]->z);
		modelMatrix *= towers[i]->roty(towers[i]->rotY);
		modelMatrix *= towers[i]->scale(0.005f, 0.005f, 0.005f);
		RenderMesh(meshes["tower"], shaders["VertexNormal"], modelMatrix);
	}


	for (i = 0; i < enemies.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);

		for (int j = 0; j < tower_bullets.size(); j++) {
			if (!enemies.empty() || !tower_bullets.empty()) {
				if (glm::distance(glm::vec3(tower_bullets[j]->x, tower_bullets[j]->y + 0.5f, tower_bullets[j]->z), glm::vec3(enemies[i]->x, enemies[i]->y + 1, enemies[i]->z)) < 4.0f)
				{
					if (enemies[i]->lives <= 1) {
						enemies[i]->dead = true;
						
					}
					else {
						enemies[i]->lives -= tower_bullets[j]->damage;
					}

					tower_bullets.erase(tower_bullets.begin() + j);
					
					if (j > 0) j--;
					if (tower_bullets.empty()) {
						break;
					}
				}
			}
		}

		for (int j = 0; j < bullets.size(); j++) {
			if (!enemies.empty() || !bullets.empty()) {
				if (glm::distance(glm::vec3(bullets[j]->x, bullets[j]->y + 0.5f, bullets[j]->z), glm::vec3(enemies[i]->x, enemies[i]->y + 1, enemies[i]->z)) < 3.0f)
				{
					if (enemies[i]->lives <= 1) {
						enemies[i]->dead = true;

					}
					else {
						enemies[i]->lives -= bullets[j]->damage;
					}

					bullets.erase(bullets.begin() + j);

					if (j > 0) j--;
					if (bullets.empty()) {
						break;
					}
				}
			}
		}

		if (enemies[i]->dead == true && enemies[i]->rotX >= PI / 2) {
			enemies.erase(enemies.begin() + i);
			if (i > 0) i--;
		}
		
		if (!enemies.empty()) {
			if (enemies[i]->x >= 15 && enemies[i]->z == -15) {
				enemies[i]->xvelocity = -0.1;
				enemies[i]->zvelocity = 0.1;
				enemies[i]->rotY = RADIANS(315);
			}

			if (enemies[i]->x <= -15 && enemies[i]->z >= 15) {
				enemies[i]->xvelocity = 0.1;
				enemies[i]->zvelocity = 0;
				enemies[i]->rotY = RADIANS(90);
			}

			if (enemies[i]->x >= 15 && enemies[i]->z >= 15) {
				enemies.erase(enemies.begin() + i);
				if (!lives.empty()) {
					lives.erase(lives.begin() + lives.size() - 1);
				}
				// TODO
			}

			//else if (enemies[i]->x >= -15 && enemies[i]->z <= 15)
			//	modelMatrix *= enemies[i]->move(enemies[i]->x - 0.1, 0, enemies[i]->z + 0.1);
			modelMatrix *= enemies[i]->move(enemies[i]->x + enemies[i]->xvelocity, 0, enemies[i]->z + enemies[i]->zvelocity);
			if (enemies[i]->dead) {
				modelMatrix *= enemies[i]->rotx(enemies[i]->rotX + 0.1f);
				modelMatrix *= enemies[i]->rotz(enemies[i]->rotZ + 0.1f);
			
			}
			modelMatrix *= enemies[i]->roty(enemies[i]->rotY);
			modelMatrix *= enemies[i]->scale(0.1f, 0.1f, 0.1f);
			RenderMesh(meshes["enemy"], shaders["VertexNormal"], modelMatrix);
		}
	}
	
	{
		
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, -0.5f, 0));

		modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));
		RenderMesh(meshes["pistol"], shaders["Simple"], modelMatrix);
		
	}

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{
		
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= player->move(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z);
	
			modelMatrix *= player->roty(angle);
		
		
		if (lives.size() == 0 && player->rotX <= PI/2) {
			modelMatrix *= player->rotx(player->rotX + 0.05f);
			modelMatrix *= player->rotz(player->rotZ + 0.05f);
		}
		else if (player->rotX > PI / 2) {
			renderCameraTarget = false;
		}
		modelMatrix *= player->scale(0.005f, 0.005f, 0.005f);
		
		RenderMesh(meshes["archer"], shaders["Simple"], modelMatrix);
	}

	// render lives
	for (i = 0; i < lives.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		lives[i]->angle += 0.01;


		switch (i) {
		case 0:
			modelMatrix *= lives[i]->move(player->x + 0.2 * cos(lives[i]->rotY), player->y + 1, player->z + 0.2 * sin(lives[i]->rotY));
			modelMatrix *= lives[i]->roty(lives[i]->angle + RADIANS(360) / 3 * i);
			break;
		case 1:
			modelMatrix *= lives[i]->move(player->x , player->y + 1 + 0.2 * cos(lives[i]->rotX), player->z + 0.2 * sin(lives[i]->rotX));
			modelMatrix *= lives[i]->rotx(lives[i]->angle + RADIANS(360) / 3 * i);
			break;
		case 2:
			modelMatrix *= lives[i]->move(player->x + 0.2 * cos(lives[i]->rotZ), player->y + 1 + 0.2 * sin(lives[i]->rotZ), player->z );
			modelMatrix *= lives[i]->rotz(lives[i]->angle + RADIANS(360) / 3 * i);

			break;
		default:
			cout << "Wrong Path";
		}
		//modelMatrix *= lives[i]->move(player->x + 0.2 * cos(lives[i]->rotY), player->y + 1, player->z + 0.2 * sin(lives[i]->rotY));
		
		modelMatrix *= lives[i]->scale(0.1f, 0.1f, 0.1f);
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
}

void Laborator5::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);


	glm::ivec2 resolution = window->GetResolution();
	// TODO
	glViewport(25, 25, 225, 225);
	// DrawScene - randez toate obiectele folosind rendermesh2
	DrawScene();

	DrawCoordinatSystem(camera2->GetViewMatrix(), projectionMatrix2 );

}

void Laborator5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Laborator5::RenderMesh2(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera2->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix2));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}



// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator5::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}
	}

	if (window->KeyHold(GLFW_KEY_0))
	{
		//fov+= 10;
		//projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_1))
	{
		
		fov = 60;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
		player->gunmode = 1;
		bullet_mode = 1;
		bullet_damage = PISTOL;
		bt = 0.5;
		if (from_build) {
	
			camera->position = aux_position;
			camera->up = aux_up;
			camera->forward = aux_forward;
			camera->right = aux_right;
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= player->move(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z);
			angle = aux_angle;
			modelMatrix *= player->roty(-angle);
			
			modelMatrix *= player->scale(0.005f, 0.005f, 0.005f);

			RenderMesh(meshes["archer"], shaders["Simple"], modelMatrix);

			from_build = false;
		}
	}

	if (window->KeyHold(GLFW_KEY_2))
	{
		fov = 15;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
		player->gunmode = 2;
		bullet_mode = 2;
		bullet_damage = SHOTGUN;
		bt = 0.25f;
		if (from_build) {
			
			camera->position = aux_position;
			camera->up = aux_up;
			camera->forward = aux_forward;
			camera->right = aux_right;
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= player->move(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z);
			angle = aux_angle;
			modelMatrix *= player->roty(-angle);
			modelMatrix *= player->scale(0.005f, 0.005f, 0.005f);

			RenderMesh(meshes["archer"], shaders["Simple"], modelMatrix);
			from_build = false;
		}
	}

	if (window->KeyHold(GLFW_KEY_3))
	{

		fov = 60;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
		player->gunmode = 3;
		bullet_mode = 3;
		bullet_damage = GRENADE;
		bt = 3;
		if (from_build) {

			camera->position = aux_position;
			camera->up = aux_up;
			camera->forward = aux_forward;
			camera->right = aux_right;
			angle = aux_angle;
			player->rotY -= aux_angle;
			glm::mat4 modelMatrix = glm::mat4(1);

			from_build = false;
		}
	}


	if (window->KeyHold(GLFW_KEY_M))
	{
		width += 2;
		projectionMatrix = glm::ortho(-width, width, -height, height, 0.01f, 250.0f);
		//projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_N))
	{
		width -= 2;
		projectionMatrix = glm::ortho(-width, width, -height, height, 0.01f, 250.0f);
		//projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}


	if (window->KeyHold(GLFW_KEY_J))
	{
		height += 2;
		projectionMatrix = glm::ortho(-width, width, -height, height, 0.01f, 250.0f);
		//projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_K))
	{
		height -= 2;
		projectionMatrix = glm::ortho(-width, width, -height, height, 0.01f, 250.0f);
		//projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

}

void Laborator5::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	if (key == GLFW_KEY_O)
	{
		projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 250.0f);
	}
	if (key == GLFW_KEY_P)
	{
		projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (key == GLFW_KEY_G)
	{
		if (!from_build) {
			player->gunmode = 4;
			aux_position = camera->position;
			aux_up = camera->up;
			aux_forward = camera->forward;
			aux_right = camera->right;
			aux_angle = player->rotY;
			camera->TranslateUpword(5);
			from_build = true;
		}
	}
}

void Laborator5::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	//if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	//{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		/*if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
		}*/
		
		//if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			//renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			if (camera->position.y >= 0 || -deltaY * sensivityOX < 0) {
				camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
			}
			//if ()
			camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
			angle += -deltaX * sensivityOY;
			//angle = -deltaX * sensivityOY;
			//player->roty(-deltaY * sensivityOX);

			
		//}

	//}
}

void Laborator5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (renderCameraTarget && bt + player_bullet < Engine::GetElapsedTime() && player->gunmode != 4) {
		float aux = atan2(player->z - (camera->position.z + camera->forward.z), player->x - (camera->position.x + camera->forward.x));
		bullets.push_back(new Bullet(0.15f, player->x, player->y + 1, player->z, aux, 0.25, bullet_damage));
		player_bullet = Engine::GetElapsedTime();
		bullets[bullets.size() - 1]->time = 0.1f;
	}

	if (player->gunmode == 4) {
		float aux = atan2(player->z - (camera->position.z + camera->forward.z), player->x - (camera->position.x + camera->forward.x));
		towers.push_back(new Tower(player->x + cos(aux), 0, player->z + sin(aux)));
	}
}

void Laborator5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator5::OnWindowResize(int width, int height)
{
}

void Laborator5::DrawScene() {
	int i;


	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= player->move(player->x, player->y, player->z);

	modelMatrix *= player->scale(0.9f, 0.9f, 0.9f);

	RenderMesh2(meshes["sphere"], shaders["Color"], modelMatrix);

	for (i = 0; i < enemies.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= enemies[i]->move(enemies[i]->x, 0, enemies[i]->z);
		

		modelMatrix *= enemies[i]->scale(1.2f, 1.2f, 1.2f);

		RenderMesh2(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	for (i = 0; i < towers.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= towers[i]->move(towers[i]->x, 0, towers[i]->z);


		modelMatrix *= towers[i]->scale(0.9f, 0.9f, 0.9f);

		RenderMesh2(meshes["sphere"], shaders["VertexColor"], modelMatrix);
	}

}
