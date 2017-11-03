#pragma once

struct  FFontData
{

};

struct FCharacterEntry
{
public:
	FCharacterEntry()
	{
		pbi = NULL;
		m_pImageBuffer = NULL;
	}
	void Release()
	{
		delete[]pbi;
		pbi = NULL;
		delete[]m_pImageBuffer;
		m_pImageBuffer = NULL;
	}
	~FCharacterEntry()
	{
		Release();
	}
	/** The character this entry is for */
	TCHAR Character;
	BITMAPINFO* pbi;
	BYTE  *m_pImageBuffer;
	float left;
	float top;

};

