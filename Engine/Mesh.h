#pragma once

#include "Object.h"

namespace ATLAT
{
class Mesh : public Object
{
public:
	virtual ~Mesh() {}

	void SetVertices(const std::vector<Vertex>& verteices);
	void SetIndeices(const std::vector<uint32>& indices);
private:
	std::wstring _name;
};
}

