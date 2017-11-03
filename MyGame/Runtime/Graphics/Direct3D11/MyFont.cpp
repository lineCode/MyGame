#include "MyFont.h"
#include <string>
#include "DeviceManager.h"
#include "VertexType.h"
#include "Sprite.h"

using namespace std;
#ifdef _DEBUG
#pragma comment(lib,"freetype2412MT_D.lib")
#else
#pragma comment(lib,"freetype2412MT.lib")
#endif


FontRender::FontRender()
{
	m_d3dDevice = NULL;
	m_deviceContext = NULL;
	m_nLeft = 0;
	m_nTop = 0;
}


FontRender::~FontRender()
{
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	std::map <wchar_t, FCharacterEntryEx >::iterator iter = MappedEntries.begin();
	for (; iter != MappedEntries.end(); ++iter)
	{
		FCharacterEntryEx& entry = (iter->second);
		entry.Release();
	}
}

void FontRender::init()
{
	m_d3dDevice = g_objDeviceManager.GetDevice();
	m_deviceContext = g_objDeviceManager.GetDeviceContext();
	m_nLeft = 0;
	m_nTop = 0;

	FT_Error bError = FT_Init_FreeType(&library);
	// 加载一个字库文件，这里为黑体中文字库   
	bError = FT_New_Face(library, "c:\\Windows\\Fonts\\msyh.ttc", 0, &face);
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	// 设定字体字符宽高和分辨率   
	bError = FT_Set_Char_Size(face, 16*64, 16 * 64, 96, 96);

	nMaxWidth = 1280;
	nMaxHeight = 768;


	D3D11_TEXTURE2D_DESC desc;
	desc.Width = static_cast<UINT>(nMaxWidth);
	desc.Height = static_cast<UINT>(nMaxHeight);
	desc.MipLevels = static_cast<UINT>(1);
	desc.ArraySize = static_cast<UINT>(1);
	desc.Format = DXGI_FORMAT_A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_d3dDevice->CreateTexture2D(&desc, NULL, &tex);
	hr = m_d3dDevice->CreateTexture2D(&desc, NULL, &tex1);


	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	memset(&SRVDesc, 0, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = m_d3dDevice->CreateShaderResourceView(tex, &SRVDesc, &m_pTexture);
	hr = m_d3dDevice->CreateShaderResourceView(tex, &SRVDesc, &m_pTexture1);



}

int FontRender::DrawString(const wstring& strText, int x, int y)
{
	int nPos = 0;
	int nLength = strText.size();
	int CharIndex = 0;;
	int nLeft = x;
	int nTop = y;
	static int nMaxFont = 1;
	if (nLength > 1300)
	{
		nMaxFont++;
	}

	nLength = min(nLength, 1300);
	if (nMaxFont >= nLength)
	{
		nMaxFont = 1;
	}
	//nLength = nMaxFont;
	int m_nWidth = nMaxWidth;
	int m_nHeight = nMaxHeight;
	std::vector<VertexPositionColorTexture> fonts;
	//fonts.resize(nLength + 5);
	int nPosCount = 0;
	bool bUpdate = false;

	if (bUpdate == false)
	{
		BeginMap();
	}
	bUpdate = true;
	for (; CharIndex < nLength; ++CharIndex)
	{
		wchar_t CurrentChar = strText[CharIndex];
		const bool IsNewline = (CurrentChar == '\n');
		if (IsNewline)
		{

		}
		else
		{
			FCharacterEntryEx& entry = MappedEntries[CurrentChar];
			if (entry.m_pImageBuffer == NULL)
			{
				GetRenderData(entry, CurrentChar);
				if (bUpdate == false)
				{
					BeginMap();
				}
				bUpdate = true;
			}
			if (bUpdate)
			{
				Update(entry);
			}
			if (entry.m_pImageBuffer != NULL)
			{
				//	continue;
				if (nLeft + entry.nWidth > nMaxWidth)
				{
					nLeft = 0;
					nTop += entry.nHeight;
					nTop += 2;

				}
				RECT pDest;
				pDest.left = nLeft;
				pDest.top = 20 - entry.top + nTop;
				pDest.right = nLeft + entry.nWidth;
				pDest.bottom = pDest.top + entry.nHeight;
				nPos = nLeft + entry.nWidth;

				float x = (float)(pDest.left);
				float y = (float)(pDest.top);
				float h = (float)(pDest.bottom - pDest.top);
				float w = (float)(pDest.right - pDest.left);
				Vector4f LeftTop = Vector4f(x, y, 0.0f, 0.0f);
				Vector4f RightTop = Vector4f(x + w, y, 0.0f, 0.0f);

				Vector4f LeftBottom = Vector4f(x, (y + h), 0.0f, 0.0f);
				Vector4f RightBottom = Vector4f(x + w, (y + h), 0.0f, 0.0f);
				float tmpX = (float)m_nWidth*1.0f / 2.0f - x;
				float tmpY = (float)m_nHeight*1.0f / 2.0f - y;
				XMFLOAT4 color(1.0f, 0.13f, 0.69f, 0.5f);
				XMFLOAT2 textureCoordinate[4];
				textureCoordinate[0].x = textureCoordinate[1].x = float(entry.pSrcRect.left) / (float)m_nWidth;
				textureCoordinate[0].y = textureCoordinate[3].y = float(entry.pSrcRect.bottom) / (float)m_nHeight;
				textureCoordinate[2].x = textureCoordinate[3].x = float(entry.pSrcRect.right) / (float)m_nWidth;
				textureCoordinate[2].y = textureCoordinate[1].y = float(entry.pSrcRect.top) / (float)m_nHeight;

				fonts.push_back({ LeftBottom, color, textureCoordinate[0] });
				fonts.push_back({ LeftTop, color, textureCoordinate[1] });
				fonts.push_back({ RightTop, color, textureCoordinate[2] });

				fonts.push_back({ LeftBottom, color, textureCoordinate[0] });
				fonts.push_back({ RightTop, color, textureCoordinate[2] });
				fonts.push_back({ RightBottom, color, textureCoordinate[3] });
				nLeft += entry.nWidth;

			}

		}
	}
	if (bUpdate)
	{
		EndMap();
	}


	//	return;
	if (fonts.size() <= 0)
	{
		return nPos;
	}


	XMMATRIX toTexSpace =  g_objSprite.GetShow2DMatrix();
	g_objSprite.DrawPrimitiveUP(PRIMITIVE_TRIANGLELIST, fonts.size(), &fonts[0], toTexSpace, m_pTexture, COLOR_TEX);
	
	return nPos;
}


void FontRender::BeginMap()
{
	std::map <wchar_t, FCharacterEntryEx >::iterator iter = MappedEntries.begin();
	m_nLeft = 0;
	m_nTop = 0;
	m_nWidhtFont = 0;
	m_nHeightFont = 0;
	m_deviceContext->Map(tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memset(mappedResource.pData, 0, nMaxWidth * nMaxHeight);
}

void FontRender::EndMap()
{
	m_deviceContext->Unmap(tex, 0);
}

void FontRender::GetRenderData(FCharacterEntryEx& InFontData, const TCHAR wChar)
{
	FT_Error bError;
	FT_UInt glyph_index = FT_Get_Char_Index(face, wChar);
	bError = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP);
	FT_GlyphSlot Slot = face->glyph;
	FT_Render_Glyph(Slot, FT_RENDER_MODE_NORMAL);
	const int GlyphPixelSize = 1;
	FT_Bitmap* ftBitmap = nullptr;
	if (Slot->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
	{
		FT_Bitmap NewBitmap;
		FT_Bitmap_New(&NewBitmap);
		// Convert the mono font to 8bbp from 1bpp
		FT_Bitmap_Convert(library, &Slot->bitmap, &NewBitmap, 4);
		ftBitmap = &NewBitmap;
	}
	else
	{
		ftBitmap = &Slot->bitmap;
	}
	InFontData.left = Slot->bitmap_left;
	InFontData.top = Slot->bitmap_top;
	InFontData.nWidth = ftBitmap->width;
	InFontData.nHeight = ftBitmap->rows;
	int nLineWidth = (InFontData.nWidth + 3) / 4 * 4; //凑成大于等于nColumn的最小的4的整倍数
	InFontData.nWidth = nLineWidth;
	InFontData.m_pImageBuffer = new BYTE[nLineWidth*InFontData.nHeight];
	int nWidth = ftBitmap->width;
	int nHeight = ftBitmap->rows;
	memset(InFontData.m_pImageBuffer, 0, nLineWidth * InFontData.nHeight);
	for (int i = 0; i < nHeight; i++)
	{
		int j = nHeight - 1 - i;
		j = i;
		memcpy(InFontData.m_pImageBuffer + i*nLineWidth, face->glyph->bitmap.buffer + j *nWidth, nWidth);
	}
	InFontData.Character = wChar;
}
void FontRender::Update(FCharacterEntryEx& InFontData)
{
	int nMax = 20;
	int nFontCount = 0;

	FCharacterEntryEx& entry = InFontData;
	if (entry.m_pImageBuffer)
	{
		int nWidth = entry.nWidth;
		int nHeight = entry.nHeight;
		m_nHeightFont = max(m_nHeightFont, nHeight);
		nMax = max(nMax, nHeight);

		if (m_nLeft + nWidth > nMaxWidth)
		{
			m_nLeft = 0;
			m_nTop += nHeight;
			m_nHeightFont += nHeight;
		}
		m_nWidhtFont += nWidth;
		int nBegin = nMax - entry.top + m_nTop;
		if (nBegin < 0 || nBegin >= nMaxHeight)
		{
			entry.pSrcRect.left = m_nLeft;
			entry.pSrcRect.top = nBegin;
			entry.pSrcRect.right = m_nLeft + nWidth;
			entry.pSrcRect.bottom = nBegin + nHeight;
			m_nLeft += nWidth;
			return;
		}
		unsigned char* pTexBuffer = (unsigned char *)mappedResource.pData;

		for (int i = 0; i < nHeight; i++)
		{
			BYTE* pSrc = (BYTE*)(entry.m_pImageBuffer + i*nWidth);
			BYTE* pDest = (BYTE*)(pTexBuffer + m_nLeft + (i + nBegin)*mappedResource.RowPitch);

			/*	for (int j = 0; j < nWidth; ++j)
			{
			pDest[0] = pSrc[0];
			pSrc += 1;
			pDest += 1;
			}*/
			memcpy(pTexBuffer + m_nLeft + (i + nBegin)*mappedResource.RowPitch, entry.m_pImageBuffer + i*nWidth, nWidth);
		}
		entry.pSrcRect.left = m_nLeft;
		entry.pSrcRect.top = nBegin;
		entry.pSrcRect.right = m_nLeft + nWidth;
		entry.pSrcRect.bottom = nBegin + nHeight;
		m_nLeft += nWidth;
	}
}

void FontRender::Update()
{
	HRESULT result;
	std::map <wchar_t, FCharacterEntryEx >::iterator iter = MappedEntries.begin();
	m_nLeft = 0;
	m_nTop = 0;
	result = m_deviceContext->Map(tex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	int nMax = 20;
	int nFontCount = 0;
	for (; iter != MappedEntries.end(); ++iter)
	{
		nFontCount++;
		if (nFontCount >= 1473)
		{
			break;;
		}
		FCharacterEntryEx& entry = (iter->second);
		if (entry.m_pImageBuffer)
		{
			int nWidth = entry.nWidth;
			int nHeight = entry.nHeight;
			nMax = max(nMax, nHeight);
			if (m_nLeft + nWidth > nMaxWidth)
			{
				m_nLeft = 0;
				m_nTop += nHeight;
			}
			int nBegin = nMax - entry.top + m_nTop;
			if (nBegin < 0 || nBegin >= nMaxHeight)
			{
				break;
			}
			unsigned char* pTexBuffer = (unsigned char *)mappedResource.pData;

			for (int i = 0; i < nHeight; i++)
			{
				BYTE* pSrc = (BYTE*)(entry.m_pImageBuffer + i*nWidth);
				BYTE* pDest = (BYTE*)(pTexBuffer + m_nLeft + (i + nBegin)*mappedResource.RowPitch);

				/*	for (int j = 0; j < nWidth; ++j)
				{
				pDest[0] = pSrc[0];
				pSrc += 1;
				pDest += 1;
				}*/
				memcpy(pTexBuffer + m_nLeft + (i + nBegin)*mappedResource.RowPitch, entry.m_pImageBuffer + i*nWidth, nWidth);
			}
			entry.pSrcRect.left = m_nLeft;
			entry.pSrcRect.top = nBegin;
			entry.pSrcRect.right = m_nLeft + nWidth;
			entry.pSrcRect.bottom = nBegin + nHeight;
			m_nLeft += nWidth;

		}
	}
	m_deviceContext->Unmap(tex, 0);
	bool bSave = true;
	if (bSave)
	{
	//	g_objTextureMgr.Save(m_pTexture, "字体.jpg");
		bSave = false;
	}
}

