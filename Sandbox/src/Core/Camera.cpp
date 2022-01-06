#include "Camera.h"

Camera::Camera() :
  Camera(Camera::defaultPosition)
{
}

Camera::Camera(glm::vec3 position) :
  Camera(position, Camera::defaultUp)
{
}

Camera::Camera(glm::vec3 position, glm::vec3 up) :
  Camera(position            ,
         up                  ,
         Camera::defaultYaw  ,
         Camera::defaultPitch)
{
}

Camera::Camera(glm::vec3 position,
               glm::vec3 up      ,
               float yaw         ,
               float pitch       ) :
    m_Position        { position                   },
    m_Front           { Camera::defaultFront       },
    m_WorldUp         { up                         },
    m_Yaw             { yaw                        },
    m_Pitch           { pitch                      },
    m_MovementSpeed   { Camera::defaultSpeed       },
    m_MouseSensitivity{ Camera::defaultSensitivity },
    m_Zoom            { Camera::defaultZoom        } 
  {
    UpdateCameraVectors();
  }

void Camera::ProcessKeyboard(MovementDirection direction, float deltaTime)
{
  float velocity = m_MovementSpeed * deltaTime;
  switch (direction)
  {
  case Camera::MovementDirection::FORWARD:
    m_Position += m_Front * velocity;
    break;
  case Camera::MovementDirection::BACKWARD:
    m_Position -= m_Front * velocity;
    break;
  case Camera::MovementDirection::RIGHT:
    m_Position += m_Right * velocity;
    break;
  case Camera::MovementDirection::LEFT:
    m_Position -= m_Right * velocity;
    break;
  default:
    break;
  }
}

void Camera::ProcessKeyboardFPS(MovementDirection direction, float deltaTime)
{
  ProcessKeyboard(direction, deltaTime);
  m_Position.y = 0.0f;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constraintPitch)
{
  xOffset *= m_MouseSensitivity;
  yOffset *= m_MouseSensitivity;

  m_Yaw += xOffset;
  m_Pitch += yOffset;

  if (constraintPitch)
  {
    if (m_Pitch > Camera::constraintPitchMax) m_Pitch = Camera::constraintPitchMax;
    if (m_Pitch < Camera::constraintPitchMin) m_Pitch = Camera::constraintPitchMin;
  }

  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
  m_Zoom -= yOffset * Camera::defaultScrollStrength;
  if (m_Zoom < Camera::minZoom) m_Zoom = Camera::minZoom;
  if (m_Zoom > Camera::maxZoom) m_Zoom = Camera::maxZoom;
}

void Camera::UpdateCameraVectors()
{
  glm::vec3 front{};
  front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  front.y = sin(glm::radians(m_Pitch));
  front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Front = glm::normalize(front);

  m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
  m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}
