#pragma once

#include "Model.h"
#include "Camera.h"

class EAngle {
public:
	EAngle();
	EAngle(float pitch, float yaw);
	
	glm::vec3 ToVector();

	void Normalize();

	float p;
	float y;
};

class Player
{
public:
	Player(glm::vec3 position);
	~Player();

	void Draw();
	void Update(float deltaTime);
	
	void MouseMotion(float x, float y);

	float Lerp(float flGoal, float flCurrent, float dt);

	Model* model;
	Camera* camera;
	Shader shader;

	EAngle angView;

	glm::vec3 position;
	glm::vec3 velocity;

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
};
