
struct LightData
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float3 direction;
    float spot;
    float3 position;
    float range;
    float3 attenuation;
    float pad;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};

void ComputeDirectionalLight(Material material, LightData light, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ambient = material.ambient * light.ambient;

	float3 lightVector = -light.direction;
	float diffuseFactor = dot(normal, lightVector);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVector, normal);
        float m = max(dot(v, toEye), 0.0f);
		float specFactor = pow(m, material.specular.w);
		diffuse = diffuseFactor * material.diffuse * light.diffuse;
		specular = specFactor * material.specular * light.specular;
	}
}

void ComputePointLight(Material material, LightData light, float3 position, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVector = light.position - position;
	float distance = length(lightVector);
	if (distance > light.range)
		return;

	lightVector /= distance;

	ambient = material.ambient * light.ambient;

	float diffuseFactor = dot(lightVector, normal);

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * material.diffuse * light.diffuse;
		float3 v = reflect(-lightVector, normal);
		float specularFactor = pow(max(dot(v, toEye), 0), material.specular.w);
		specular = specularFactor * material.specular * light.specular;
	}
	float attenuation = 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));
	diffuse *= attenuation;
	specular *= attenuation;
}

void ComputeSpotLight(Material material, LightData light, float3 position, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVector = light.position - position;
	float distance = length(lightVector);

	if (distance > light.range)
		return;

	ambient = material.ambient * light.ambient;
	
    lightVector /= distance;
	
	float diffuseFactor = dot(normal, lightVector);
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * material.diffuse * light.diffuse;
		float3 v = reflect(-lightVector, normal);
		float specularFactor = pow(max(dot(v, normal), 0.0f), material.specular.w);
		specular = specularFactor * material.specular * light.specular;
	}
	float spot = pow(max(dot(-lightVector, light.direction), 0.0f), light.spot);
	float attenuation = spot / dot(light.attenuation, float3(1.0f, distance, distance * distance));
	ambient *= spot;
	diffuse *= attenuation;
	specular *= attenuation;
}