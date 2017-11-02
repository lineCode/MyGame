#pragma once
#include "PlatformInterface.h"
#include <d3d11.h>
#include <dxgi.h>
#include <vector>


enum ShaderType
{
	VS = 0,
	HS,
	DS,
	GS,
	GS_OUTPUT,
	PS,
	CS,
	MAX_SHADER
};

/// Texture units.
enum TextureUnit
{
	TEXTURE0= 0,
	TEXTURE1 = 1,
	TEXTURE2 = 2,
	TEXTURE3 = 2,
	MAX_TEXTURE_UNITS = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT
};


