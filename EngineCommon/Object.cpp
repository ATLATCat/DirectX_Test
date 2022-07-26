#include "pch.h"
#include "Object.h"

ATLAT::Object::Object()
	: _uid(0)
{
	_uid = g_uuidCount++;
}

ATLAT::Object::~Object()
{
}
