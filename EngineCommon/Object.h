#pragma once

namespace ATLAT
{
class Object
{
public:
	Object();
	virtual ~Object();

	UID GetUID() { return _uid; }
	void SetUID(UID uid) { _uid = uid; }
private:
	UID _uid;
};
}

