#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	this->position = position;
	this->frontVector = glm::normalize(target - position);
	this->rightVector = glm::cross(this->frontVector, worldUp);
	this->upVector = glm::cross(rightVector, frontVector);
	this->worldUp = worldUp;
	this->lookAtMatrix = glm::lookAt(this->position, this->position + this->frontVector, this->worldUp);
}

void Camera::updateCameraDirection(double diffMousePositionX, double diffMousePositionY, float movementSensitivity)
{
	yaw += (float)(diffMousePositionX) * movementSensitivity;
	pitch += (float)(diffMousePositionY) * movementSensitivity;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVector = glm::normalize(direction);

	this->updateCameraVectors();
}

void Camera::updateCameraPosition(CameraMovement movementDirection, float amount)
{
	if(movementDirection == CameraMovement::FORWARD)
	{
		this->position += amount * frontVector;
	}
	if(movementDirection == CameraMovement::BACKWARD)
	{
		this->position -= amount * frontVector;
	}
	if(movementDirection == CameraMovement::LEFT)
	{
		this->position -= glm::normalize(glm::cross(this->frontVector, this->upVector)) * amount;
		//this->position += -this->rightVector * amount;
	}
	if(movementDirection == CameraMovement::RIGHT)
	{
		this->position += glm::normalize(glm::cross(this->frontVector, this->upVector)) * amount;
		//this->position += this->rightVector * amount;
	}

	//updateLookAtMatrix
	this->lookAtMatrix = glm::lookAt(this->position, this->position + this->frontVector, this->worldUp);
}


void Camera::updateCameraVectors()
{
	this->rightVector = glm::cross(this->frontVector, worldUp);
	this->upVector = glm::cross(rightVector, frontVector);
	this->lookAtMatrix = glm::lookAt(this->position, this->position + this->frontVector, this->worldUp);
}

void Camera::updateCameraZoom(float amount)
{
	zoom -= (float)amount;

	if (zoom < 1.0f)
	{
		zoom = 1.0f;
	}

	if (zoom > 45.0f)
	{
		zoom = 45.0f;
	}
	
}
