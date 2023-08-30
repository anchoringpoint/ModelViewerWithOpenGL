#version 460 core

#define MAX_TEXTURE_NUM 8

struct Material
{
	sampler2D texture_diffuse[MAX_TEXTURE_NUM];
	sampler2D texture_specular[MAX_TEXTURE_NUM];
	int texture_diffuse_num;
	int texture_specular_num;
	float shininess;
};

// �����
struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// ���Դ
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

// �۹�-�ֵ�Ͳ
struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// ��Բ׶����ֵ
	float cutOff;
	// ��Բ׶����ֵ
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;  

uniform vec3 viewPos;

uniform Material material;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalLightSum(vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, float diff, float spec);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// �������
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// ���Դ
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalPointLight(pointLights[i], norm, FragPos, viewDir);
	// �۹�
	//result += CalSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

// ����ĳ�������Դ�Ը�Ƭ����ɫ�Ĺ���
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// ��������ɫ
	float diff = max(dot(normal,lightDir),0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// �ϲ�
	return CalLightSum(light.ambient, light.diffuse, light.specular, diff, spec);
}

// ����ĳ�����Դ�Ը�Ƭ����ɫ�Ĺ���
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// ��������ɫ
	float diff = max(dot(normal,lightDir),0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// ����
	float dis = length(light.position - fragPos);
	// ˥��
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis *dis);
	// �ϲ�
	return CalLightSum(light.ambient, light.diffuse, light.specular, diff, spec) * attenuation;
}

// ����ĳ���۹��Դ�Ը�Ƭ����ɫ�Ĺ���
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// ��������ɫ
	float diff = max(dot(normal,lightDir),0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// ����
	float dis = length(light.position - fragPos);
	// ˥��
	float attenuation = 1.0 /(light.constant + light.linear * dis + light.quadratic * dis * dis);
	// ������нǲ�ֵ ʵ��ƽ������Ч��
	float cosTheta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((cosTheta  - light.outerCutOff) / epsilon, 0.0, 1.0);
	// �ϲ�
	return CalLightSum(light.ambient, light.diffuse, light.specular, diff, spec) * attenuation * intensity;
}

// ���㻷���⡢�����䡢��������ɫ��
// û�м���˥��
vec3 CalLightSum(vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular, float diff, float spec)
{
	vec3 diffuse = vec3(0.0,0.0,0.0);
	for(int i = 0; i < material.texture_diffuse_num; i++)
	{
		diffuse += texture(material.texture_diffuse[i], TexCoords).rgb;
	}
	vec3 ambient = lightAmbient * diffuse;
	diffuse = lightDiffuse * diff * diffuse;
	vec3 specular = vec3(0.0,0.0,0.0);
	for(int i = 0; i < material.texture_specular_num; i++)
	{
		specular += texture(material.texture_specular[i], TexCoords).rgb;
	}
	specular = lightSpecular * spec * specular;
	return ambient + diffuse + specular;
}

