#pragma once

#include "LookAt.h"

glm::mat4 LookAt(glm::vec3 position,
                 glm::vec3 target,
                 glm::vec3 worldUp)
{
  glm::vec3 zAxis = glm::normalize(position - target);
  glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(worldUp), zAxis));
  glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));

  glm::mat4 translation{ 1.0f };
  translation[3][0] = -position.x;
  translation[3][1] = -position.y;
  translation[3][2] = -position.z;

  glm::mat4 rotation{ 1.0f };
  rotation[0][0] = xAxis.x;
  rotation[1][0] = xAxis.y;
  rotation[2][0] = xAxis.z;

  rotation[0][1] = yAxis.x;
  rotation[1][1] = yAxis.y;
  rotation[2][1] = yAxis.z;

  rotation[0][2] = zAxis.x;
  rotation[1][2] = zAxis.y;
  rotation[2][2] = zAxis.z;

  return rotation * translation;
}