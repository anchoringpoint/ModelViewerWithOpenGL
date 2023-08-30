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

// 方向光
struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// 点光源
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

// 聚光-手电筒
struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 内圆锥余弦值
	float cutOff;
	// 外圆锥余弦值
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
	
	// 定向光照
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// 点光源
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalPointLight(pointLights[i], norm, FragPos, viewDir);
	// 聚光
	//result += CalSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

// 计算某个方向光源对该片段颜色的贡献
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// 漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// 合并
	return CalLightSum(light.ambient, light.diffuse, light.specular, diff, spec);
}

// 计算某个点光源对该片段颜色的贡献
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// 距离
	float dis = length(light.position - fragPos);
	// 衰减
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis *dis);
	// 合并
	return CalLightSum(light.ambient, light.diffuse, light.specular, diff, spec) * attenuation;
}

// 计算某个聚光灯源对该片段颜色的贡献
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	// 距离
	float dis = length(light.position - fragPos);
	// 衰减
	float attenuation = 1.0 /(light.constant + light.linear * dis + light.quadratic * dis * dis);
	// 内外光切角插值 实现平滑过度效果
	float cosTheta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((cosTheta  - light.outerCutOff) / epsilon, 0.0, 1.0);
	// 合并
	return CalLightSum(light.ambient, light.diffuse, light.specular, diff, spec) * attenuation * intensity;
}

// 计算环境光、漫反射、镜面光的颜色和
// 没有计算衰减
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

