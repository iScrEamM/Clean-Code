#include "Player.h"

Player::Player(glm::vec3 position)
{
	this->position = position;
	this->velocity = glm::vec3(0.01f, 0.01f, 0.01f);

	model = new Model("Models/Nanosuit/nanosuit.obj");
	camera = new Camera(position, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 20.0f, 100000.0f);

	shader.CreateShaderProgram("Shader/modelShader.vs.glsl", "Shader/modelShader.fs.glsl");
	shader.useProgram();
}


Player::~Player()
{
	delete model;
	delete camera;
}

void Player::Draw()
{
	model->Draw(shader);
}

void Player::Update(float deltaTime)
{
	glm::vec3 moveSpeed = glm::vec3(0.0f);
	glm::vec3 goal = glm::vec3(0.0f);

	if (keyState[SDL_SCANCODE_W])
		goal.z = -2.0f;
	if (keyState[SDL_SCANCODE_S])
		goal.z = 2.0f;
	if (keyState[SDL_SCANCODE_A])
		goal.x = -2.0f;
	if (keyState[SDL_SCANCODE_D])
		goal.x = 2.0f;
	if (keyState[SDL_SCANCODE_SPACE])
		goal.y = 2.0f;
	if (keyState[SDL_SCANCODE_LSHIFT])
		goal.y = -2.0f;

	moveSpeed.x = Lerp(goal.x, moveSpeed.x, deltaTime);
	moveSpeed.y = Lerp(goal.y, moveSpeed.y, deltaTime);
	moveSpeed.z = Lerp(goal.z, moveSpeed.z, deltaTime);

	velocity = moveSpeed;
	position += velocity;

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 1000000000.0f);

	glm::mat4 view = glm::mat4(1.0f);
	view = camera->getViewMatrix();

	glm::mat4 model = glm::mat4(1.0f); {
		glm::mat4 translate = glm::translate(model, position);
		glm::mat4 rotate = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 scale = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		model = translate * rotate * scale;
	}
	camera->UpdateTargetPosition(position);

	glm::mat4 mvp = projection * view * model;
	GLint u_mvp = shader.GetUniformLocation("u_mvp");
	glUniformMatrix4fv(u_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void Player::MouseMotion(float x, float y)
{
	camera->ProcessMouseMovement(x, y, 0.1f, true);
}


float Player::Lerp(float flGoal, float flCurrent, float dt)
{
	float flDifference = flGoal - flCurrent;

	if (flDifference > dt)
		return flCurrent + dt;
	if (flDifference < -dt)
		return flCurrent - dt;

	return flGoal;
}
