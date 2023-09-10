#version 420 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;	//模型
uniform mat4 view;	//观察
uniform mat4 projection;	//投影

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    // 注意乘法要从右向左读
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
