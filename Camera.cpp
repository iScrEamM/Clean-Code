#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, float dist)
{
	this->m_Dist = dist;
	this->position = position;
	this->m_WorldUp = up;

	this->m_Yaw = yaw;
	this->m_Pitch = pitch;
	//this->UpdateCameraVectors();
	UpdateViewMatrix(true);
}

glm::mat4 Camera::getViewMatrix() 
{
	return viewMat;	//return glm::lookAt(position, position + m_Front, up);
}

/*void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	float velocity = moveSpeed * deltaTime;

	switch (direction) {
		case FORWARD:   position += m_Front   * velocity; break;
		case BACKWARD:  position -= m_Front   * velocity; break;
		case LEFT:      position -= m_Right   * velocity; break;
		case RIGHT:     position += m_Right   * velocity; break;
		case UPWARDS:   position += m_WorldUp * velocity; break;
		case DOWNWARDS: position -= m_WorldUp * velocity; break;
	}
}

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (constrainPitch) {
		if (m_Pitch > 89.0f) {
			m_Pitch = 89.0f;
		} else if (m_Pitch < -89.0f) {
			m_Pitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}
void Camera::ProcessMouseScroll(GLfloat offset) 
{
	if (offset != 0 && m_FOV >= 1.0f && m_FOV <= FOV) {
		m_FOV -= offset;
	}
	if (m_FOV < 1.0f) {
		m_FOV = 1.0f;
	} else if (m_FOV > FOV) {
		m_FOV = FOV;
	}
}

glm::vec3 Camera::cross(glm::vec3 v1, glm::vec3 v2)
{
	glm::vec3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

glm::vec3 Camera::getEAngles()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	return front;
}

void Camera::UpdateCameraVectors() 
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = -sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	up = glm::normalize(glm::cross(m_Right, m_Front));
}
*/

void Camera::UpdateViewMatrix(bool computeDir)
{
	if (computeDir) {
		glm::vec3 radial;
		radial.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		radial.y = -sin(glm::radians(m_Pitch));
		radial.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Dir = -radial;
	}

	glm::vec3 pos = position - m_Dist * m_Dir;

	viewMat = glm::lookAt(position, pos, m_WorldUp);
}

void Camera::UpdateTargetPosition(glm::vec3 const & pos)
{
	position = pos;
	UpdateViewMatrix();
}

void Camera::UpdateAngles(GLfloat yaw, GLfloat pitch, GLboolean constrainPitch)
{
	if (constrainPitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}

	if (yaw < -180.0f || yaw > 180.0f) {
		yaw -= floor((yaw + 180.0f) / 360.0f) * 360.0f;
	}

	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateViewMatrix(true);
}

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLfloat sensitivity, GLboolean constrainPitch)
{
	UpdateAngles(m_Yaw + xOffset * sensitivity,
		m_Pitch + yOffset * sensitivity,
		constrainPitch);
}