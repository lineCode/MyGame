#pragma once
#include "dxbase.h"
#include "ft2build.h"
// Freetype style include
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_BITMAP_H

#include <map>
#include <string>
using namespace std;

class FontRender
{
	struct FCharacterEntryEx
	{
		FCharacterEntryEx()
		{
			m_pImageBuffer = NULL;
		}
		void Release()
		{
			delete[]m_pImageBuffer;
			m_pImageBuffer = NULL;
		}
		~FCharacterEntryEx()
		{
			Release();
		}
		wchar_t Character;
		int left;
		int top;
		int nWidth;
		int nHeight;
		RECT pSrcRect;
		BYTE  *m_pImageBuffer;
	};

public:
	FontRender();
	~FontRender();
	int DrawString(const wstring& strText, int x, int y);
	void BeginMap();
	void EndMap();
	void GetRenderData(FCharacterEntryEx& InFontData, const TCHAR Char);
	void Update();
	void Update(FCharacterEntryEx& InFontData);
	void init();
private:

	FT_Library library;
	FT_Face face;
	std::map<wchar_t, FCharacterEntryEx> MappedEntries;
	ID3D11Device			*m_d3dDevice;				//D3D11设备
	ID3D11DeviceContext		*m_deviceContext;			//设备上下文
	ID3D11Texture2D *tex;
	ID3D11ShaderResourceView* m_pTexture;

	ID3D11Texture2D *tex1;
	ID3D11ShaderResourceView* m_pTexture1;

	int nMaxWidth;
	int nMaxHeight;
	int m_nLeft;
	int m_nTop;
	int m_nWidhtFont;
	int m_nHeightFont;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
};

