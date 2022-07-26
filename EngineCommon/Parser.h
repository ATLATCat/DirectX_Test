#pragma once

#include "Lexer.h"

namespace ASEParser
{
using namespace ATLAT;
struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
	Vector3 tangent;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32> indices;
};

struct GameObject
{
	std::string name;
	std::vector<std::shared_ptr<GameObject>> children;
	std::shared_ptr<Mesh> mesh;
	Matrix matrix;
	std::shared_ptr<GameObject> parent;
};

struct Material
{
	std::wstring shaderName;
	std::wstring textureName;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 reflect;
};

class Parser
{
public:
	void LoadFile(const char* filename);
	void Parse(std::wstring filePath);
	void Clear();
	void ObjectClear();

	void UpdateLocalTransform();
	
	void ReverseClockWise();

	std::vector<std::shared_ptr<Mesh>> _meshes;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _verticesOrigin;
	std::vector<Vector2> _textureUV;
	std::vector<UINT> _textureIndices;
	std::vector<UINT> _indices;
	std::vector<UINT> _indicesOrigin;
	std::vector<std::shared_ptr<Material>> _materials;
	int _meshIndex = -1;
	int _faceNormalIndex;
	int _faceNormalCount;
	int _materialIndex;

	std::vector<std::shared_ptr<GameObject>> _gameObjects;
	std::vector<std::shared_ptr<GameObject>> _rootObjects;

	std::string _nodeName;
	std::string _nodeParent;
	Matrix _nodeMatrix;
};
}
