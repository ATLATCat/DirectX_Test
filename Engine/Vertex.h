#pragma once

namespace ATLAT
{

namespace VertexSemanticName
{
	LPCSTR const Position = "POSITION";
	LPCSTR const Color = "COLOR";
	LPCSTR const Normal = "NORMAL";
	LPCSTR const Texture = "TEXCOORD";
}

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

struct VertexPosColor
{
	Vector3 position;
	Vector4 color;
};

struct VertexPosTex
{
	Vector3 position;
	Vector2 texture;
};

struct VertexPosNormal 
{
	Vector3 position;
	Vector3 normal;
};

struct VertexPosNormalTex
{
	VertexPosNormalTex() { ZeroMemory(this, sizeof(this)); }

	Vector3 position;
	Vector3 normal;
	Vector2 texture;
};
}