#pragma once

#include "Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  enum class MovementDirection
    : std::int32_t
  {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
  };

  Camera();
  Camera(glm::vec3 position);
  Camera(glm::vec3 position, glm::vec3 up);
  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

  inline glm::mat4 GetViewMatrix() const
  {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
  }

  inline glm::vec3 GetPosition() const 
  {
    return m_Position;
  }

  inline float GetZoom() const 
  {
    return m_Zoom;
  }

  void ProcessKeyboard(MovementDirection direction, float deltaTime);
  void ProcessKeyboardFPS(MovementDirection direction, float deltaTime);
  void ProcessMouseMovement(float xOffset, float yOffset, bool constraintPitch = true);
  void ProcessMouseScroll(float yOffset);

private:
  void UpdateCameraVectors();

  static constexpr float defaultYaw            = -90.0f;

  static constexpr float defaultPitch          = 0.0f;
  static constexpr float constraintPitchMin    = -89.0f;
  static constexpr float constraintPitchMax    = 89.0f;

  static constexpr float defaultSpeed          = 2.5f;
  static constexpr float defaultSensitivity    = 0.1f;
  static constexpr float defaultScrollStrength = 2.0f;

  static constexpr float defaultZoom           = 45.0f;
  static constexpr float minZoom               = 1.0f;
  static constexpr float maxZoom               = defaultZoom;

  static constexpr glm::vec3 defaultPosition   = glm::vec3{ 0.0f, 0.0f, 0.0f };
  static constexpr glm::vec3 defaultUp         = glm::vec3{ 0.0f, 1.0f, 0.0f };
  static constexpr glm::vec3 defaultFront      = glm::vec3{ 0.0f, 0.0f, -1.0f };

  glm::vec3 m_Position;
  glm::vec3 m_Front;
  glm::vec3 m_Up;
  glm::vec3 m_Right;
  glm::vec3 m_WorldUp;

  float m_Yaw;
  float m_Pitch;

  float m_MovementSpeed;
  float m_MouseSensitivity;
  float m_Zoom;

};

