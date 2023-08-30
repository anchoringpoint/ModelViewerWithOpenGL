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
// �۹�-�ֵ�Ͳ
struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // ��Բ׶����ֵ
    float cutOff;
    // ��Բ׶����ֵ
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};