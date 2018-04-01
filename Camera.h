#pragma once

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>
#include "Shader.h"

class Camera 
{
public:

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UPWARDS,
		DOWNWARDS
	};

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		   glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
		   GLfloat yaw        = -90.0f,
		   GLfloat pitch      = 0.0f,
		   float dist         = 0.0f);

	glm::mat4 getViewMatrix();

	//void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	//void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

	void UpdateViewMatrix(bool computeDir = false);
	void UpdateTargetPosition(glm::vec3 const & pos);

	//void ProcessMouseScroll(GLfloat yOffset);

	inline GLfloat getYaw()               const { return m_Yaw; }
	inline GLfloat getPitch()             const { return m_Pitch; }
	inline const glm::vec3& getPosition() const { return position; }

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 m_WorldUp = glm::vec3(0.0f);
	glm::vec3 m_Dir;

	glm::mat4 viewMat = glm::mat4(1.0f);

	float m_Dist;

	GLfloat m_Yaw = 0.0f;
	GLfloat m_Pitch = 0.0f;

	void UpdateAngles(GLfloat yaw, GLfloat pitch, GLboolean constrainPitch);
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLfloat sensitivity, GLboolean constrainPitch);


	//void UpdateCameraVectors();
};

