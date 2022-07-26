
#include "LightHelper.hlsli"

cbuffer Light : register(b0)
{
    LightData g_light;
};

cbuffer Transform : register(b1)
{
    float4x4 g_world;
    float4x4 g_worldView;
    float4x4 g_worldViewProjMatrix;
    float3 g_viewPosition;
    float pad;
};

cbuffer Material : register(b2)
{
    Material g_material;
}

Texture2D tex_0 : register(t0);
SamplerState sam_0 : register(s0);

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 positionH : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.positionH = mul(float4(input.pos, 1.f), g_worldViewProjMatrix);
    output.uv = input.uv;
    output.normalW = mul(float4(input.normal, 0.f), g_world);
    output.positionW = mul(float4(input.pos, 1.0f), g_world).xyz;
    
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    input.normalW = normalize(input.normalW);
	
    float3 toEyeW = normalize(g_viewPosition - input.positionW);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 A, D, S;

    ComputeDirectionalLight(g_material, g_light, input.normalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;

    float4 litColor = ambient + diffuse + specular;
    litColor = tex_0.Sample(sam_0, input.uv) * litColor;
    litColor.a = g_material.diffuse.a;
    return litColor;
}
