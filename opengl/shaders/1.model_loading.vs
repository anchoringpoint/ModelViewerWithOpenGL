#version 420 core
layout (location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;	//ģ��
uniform mat4 view;	//�۲�
uniform mat4 projection;	//ͶӰ

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    // ע��˷�Ҫ���������
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
