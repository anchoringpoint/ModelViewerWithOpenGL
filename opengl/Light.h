#pragma once
#include <glm/glm.hpp>

struct PointLight
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
struct DirLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
// 聚光-手电筒
struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // 内圆锥余弦值
    float cutOff;
    // 外圆锥余弦值
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};