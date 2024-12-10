module;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Ray.h"
export module Camera;

export enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

export struct CameraState
{
	glm::vec3 position;
	glm::vec3 frontVector;
	glm::vec3 rightVector;
	glm::vec3 upVector;
	glm::vec3 worldUp;
	glm::mat4 lookAtMatrix;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float zoom = 45.0f;
};

export class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target = glm::vec3(0.0, 0.0, 0.0), glm::vec3 worldUp = glm::vec3(0.0, 1.0, 0.0));

	void updateCameraDirection(double diffMousePositionX, double diffMousePositionY);
	void updateCameraPosition(CameraMovement movementDirection);
	void updateCameraVectors();

	CameraState getState();
private:
	CameraState m_state;
	//Ray m_cameraRay;
};