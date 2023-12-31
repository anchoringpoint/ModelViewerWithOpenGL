#version 460 core
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;	//模型
uniform mat4 view;	//观察
uniform mat4 projection;//投影

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
