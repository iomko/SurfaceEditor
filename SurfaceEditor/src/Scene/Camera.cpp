module;
#include <iostream>
#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EditorSettings.h"
module Camera;


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	m_state.position = position;
	m_state.frontVector = glm::normalize(target - position);
	m_state.rightVector = glm::cross(m_state.frontVector, worldUp);
	m_state.upVector = glm::cross(m_state.rightVector, m_state.frontVector);
	m_state.worldUp = worldUp;
	m_state.lookAtMatrix = glm::lookAt(m_state.position, m_state.position + m_state.frontVector, m_state.worldUp);
}

void Camera::updateCameraDirection(double diffMousePositionX, double diffMousePositionY)
{
	m_state.yaw += (float)(diffMousePositionX) * EditorSettings::CameraSettings::m_rotationSensitivity;
	// -diffMousePositionY (because MouseDY is inverted)
	m_state.pitch += (float)(-diffMousePositionY) * EditorSettings::CameraSettings::m_rotationSensitivity;

	if (m_state.pitch > 89.0f)
	{
		m_state.pitch = 89.0f;
	}
	if (m_state.pitch < -89.0f)
	{
		m_state.pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_state.yaw)) * cos(glm::radians(m_state.pitch));
	direction.y = sin(glm::radians(m_state.pitch));
	direction.z = sin(glm::radians(m_state.yaw)) * cos(glm::radians(m_state.pitch));
	m_state.frontVector = glm::normalize(direction);

	this->updateCameraVectors();
}

void Camera::updateCameraPosition(CameraMovement movementDirection)
{
	if (movementDirection == CameraMovement::FORWARD)
	{
		m_state.position += EditorSettings::CameraSettings::m_movementSensitivity * m_state.frontVector;
	}
	if (movementDirection == CameraMovement::BACKWARD)
	{
		m_state.position -= EditorSettings::CameraSettings::m_movementSensitivity * m_state.frontVector;
	}
	if (movementDirection == CameraMovement::LEFT)
	{
		m_state.position += -m_state.rightVector * EditorSettings::CameraSettings::m_movementSensitivity;
	}
	if (movementDirection == CameraMovement::RIGHT)
	{
		m_state.position += m_state.rightVector * EditorSettings::CameraSettings::m_movementSensitivity;
	}

	// Update LookAt matrix
	m_state.lookAtMatrix = glm::lookAt(m_state.position, m_state.position + m_state.frontVector, m_state.worldUp);
}

void Camera::updateCameraVectors()
{
	m_state.rightVector = glm::cross(m_state.frontVector, m_state.worldUp);
	m_state.upVector = glm::cross(m_state.rightVector, m_state.frontVector);
	m_state.lookAtMatrix = glm::lookAt(m_state.position, m_state.position + m_state.frontVector, m_state.worldUp);
}

CameraState Camera::getState()
{
	return m_state;
}
