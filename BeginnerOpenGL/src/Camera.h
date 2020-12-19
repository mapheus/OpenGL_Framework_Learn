#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_Position = glm::vec3(0,10,-10);
	glm::vec3 m_Front = glm::vec3(0, 0, -1);
	glm::vec3 m_Up = glm::vec3(0, 1, 0);
	glm::vec3 m_Right = glm::vec3(1, 0, 0);
	glm::vec3 m_WorldUp = glm::vec3(0, 1, 0);

	float m_Yaw;
	float m_Pitch;
	// camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;

	
public:
	Camera(float fov, float width, float height);
	~Camera();

	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewMatrix() const; 
	const glm::mat4& GetViewProjectionMatrix() const;
	const glm::vec3& GetPosition() const { return m_Position; }
	void UpdateCameraVectors();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xdoffset, float yoffset, bool constrainPitch = true);
};