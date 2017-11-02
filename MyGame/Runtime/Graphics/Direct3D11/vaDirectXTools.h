#pragma once
/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "dxbase.h"

class vaDirectXTools
{
public:
	vaDirectXTools();
	~vaDirectXTools();
	void vaDirectXTools_OnDeviceCreated();
	void vaDirectXTools_OnDeviceDestroyed();
public:
	static void                         NameObject(ID3D11DeviceChild * object, const char * permanentNameString);
	static void                         NameObject(ID3D11Resource * resourceobject, const char * permanentNameString);

};

