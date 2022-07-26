
cbuffer Transform : register(b1)
{
    float4x4 g_world;
    float4x4 g_worldView;
    float4x4 g_worldViewProjMatrix;
    float3 g_viewPosition;
    float pad;
};


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
    
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
}
