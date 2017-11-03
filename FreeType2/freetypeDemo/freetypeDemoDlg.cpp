
// freetypeDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "freetypeDemo.h"
#include "freetypeDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfreetypeDemoDlg 对话框



CfreetypeDemoDlg::CfreetypeDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfreetypeDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CfreetypeDemoDlg::~CfreetypeDemoDlg()
{
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	std::map <wchar_t, FCharacterEntry >::iterator iter = MappedEntries.begin();
	for (; iter != MappedEntries.end();++iter)
	{
		FCharacterEntry& entry = (iter->second);
		entry.Release();
	}
}

void CfreetypeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CfreetypeDemoDlg::DrawString(const wstring& strText, HDC hdc)
{
	int nLength = strText.size();
	int CharIndex = 0;;
	int nLeft = 0;
	int nTop = 0;
	CRect rect;
	GetClientRect(&rect);
	int nMaxWidth = rect.Width();
	int nMaxHeight = rect.Height();

	for (; CharIndex < nLength; ++CharIndex)
	{
		TCHAR CurrentChar = strText[CharIndex];
		const bool IsNewline = (CurrentChar == '\n');
		if (IsNewline)
		{

		}
		else
		{
			FCharacterEntry& Entry = MappedEntries[CurrentChar];
			if (Entry.pbi == NULL)
			{
				GetRenderData(Entry, CurrentChar);
			}
			if (Entry.pbi == NULL)
			{
				continue;
			}
			int nWidth = Entry.pbi->bmiHeader.biWidth;
			int nHeight = Entry.pbi->bmiHeader.biHeight;
			int nWidth1 = Entry.pbi->bmiHeader.biWidth;
			int nHeight2 = Entry.pbi->bmiHeader.biHeight;
			// nWidth = Entry.pbi->bmiHeader.biWidth;
		
			 nTop =/* nHeight - */Entry.top;
			 nTop /= 2;
			// nHeight = Entry.top;
			if (nLeft + nWidth > nMaxWidth)
			{
				nLeft = 0;
				nTop += nHeight;
				nTop += 1;
			}
			SetStretchBltMode(hdc, COLORONCOLOR);  //这句要注意。
			::StretchDIBits(hdc, nLeft, nTop, nWidth, nHeight, 0, 0, nWidth1, nHeight2,
				(const void*)Entry.m_pImageBuffer, Entry.pbi, DIB_RGB_COLORS, SRCCOPY);
			nLeft += nWidth;
			nLeft += 1;
		}
	}
}

void CfreetypeDemoDlg::GetRenderData(FCharacterEntry& InFontData, const TCHAR wChar)
{
	FT_Error bError;
	FT_UInt glyph_index = FT_Get_Char_Index(face, wChar);
	float InScale = 160.0f;
	//FT_Error Error = FT_Set_Char_Size(face, 0, InScale * 64, 96, 96);
	bError = FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_BITMAP);
	FT_GlyphSlot Slot = face->glyph;
	FT_Render_Glyph(Slot, FT_RENDER_MODE_NORMAL);

	InFontData.left = Slot->bitmap_left;
	InFontData.top = Slot->bitmap_top;

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
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = face->glyph->bitmap.width;
	bi.biHeight = face->glyph->bitmap.rows;
	bi.biPlanes = 1;
	bi.biBitCount = 8;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	BITMAPINFO* pbi = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)];
	pbi->bmiHeader = bi;
	// 初始化成员
	for (int i = 0; i < 256; i++)
	{
		pbi->bmiColors[i].rgbBlue = i;
		pbi->bmiColors[i].rgbGreen = i;
		pbi->bmiColors[i].rgbRed = i;
		pbi->bmiColors[i].rgbReserved = 0;
	}

	int nWidth = bi.biWidth;
	int nHeight = bi.biHeight;
	int nLineWidth = (nWidth + 3) / 4 * 4; //凑成大于等于nColumn的最小的4的整倍数

	InFontData.m_pImageBuffer = new BYTE[nLineWidth*nHeight];
	InFontData.Character = wChar;
	InFontData.pbi = pbi;

	for (int i = 0; i < nHeight; i++)
	{
		int j = nHeight - 1 - i;
		memcpy(InFontData.m_pImageBuffer + i*nLineWidth, face->glyph->bitmap.buffer + j *nWidth, nWidth);
	}
	int RowPitch = 1 * nWidth;
	RowPitch = ftBitmap->rows;
	static int bSave = TRUE;
	if (bSave)
	{
		int bpp = 4 * 8;
		FIBITMAP *newDib = FreeImage_AllocateT(FIT_BITMAP, nWidth, nHeight, bpp);
		int pitch = FreeImage_GetPitch(newDib);
		unsigned char* pSrcBuf = (unsigned char *)face->glyph->bitmap.buffer;
		for (int i = 0; i < (int)nHeight; ++i)
		{
			int j = nHeight - 1 - i;
			unsigned char *bits = FreeImage_GetBits(newDib) + j *pitch;
			unsigned char* src = pSrcBuf + (i*nWidth);
			for (int x = 0; x < nWidth; x++)
			{
				int nColor = src[0];
				//BGRA
				bits[0] = nColor;
				bits[1] = nColor;
				bits[2] = nColor;
				bits[3] = 255.0f;
				bits += 4;
				src += 1;
			}
		}
		FreeImage_Save(FIF_PNG, newDib, "12.jpg");
		FreeImage_Unload(newDib);
		bSave = FALSE;
	}
}

BEGIN_MESSAGE_MAP(CfreetypeDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CfreetypeDemoDlg 消息处理程序

BOOL CfreetypeDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	FT_Error bError = FT_Init_FreeType(&library);

	 // 加载一个字库文件，这里为黑体中文字库   
	bError = FT_New_Face(library, "STHUPO.TTF", 0, &face);
	if (bError == FT_Err_Unknown_File_Format)
	{
		return TRUE;
	}
	else if (bError)
	{
		return TRUE;
	}

	FT_Select_Charmap(face, FT_ENCODING_UNICODE);


	// 设定字体字符宽高和分辨率   


	bError = FT_Set_Char_Size(face, 0, 5*16 * 64, 96, 96);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfreetypeDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfreetypeDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC* pDC = GetDC();
		CRect rect;
		GetClientRect(&rect);
		CBrush brush;
		if (!brush.CreateSolidBrush(RGB(0, 0, 0)))
			return;
		pDC->FillRect(rect, &brush);
		brush.UnrealizeObject();//reset a logical palette 
		FT_Error bError;
		FT_CharMap map = face->charmap;
		wstring string = L"";
		wchar_t a = '一';
		//4e00  --		9fa5
		//19968  -- 40869
		//string += L"刘武密And余惠平";

		for (int i = 19968; i < 30869; ++i)
		{
			string += i;
		}
	//	DrawString(string, pDC->m_hDC);
	//	DrawString(L"雷郑动", pDC->m_hDC);
	DrawString(L"中文and英文", pDC->m_hDC);

		ReleaseDC(pDC);

/*
		if (InScale != 1.0f)
		{
			FT_Matrix ScaleMatrix;
			ScaleMatrix.xy = 0;
			ScaleMatrix.xx = (FT_Fixed)(InScale * 65536);
			ScaleMatrix.yy = (FT_Fixed)(InScale * 65536);
			ScaleMatrix.yx = 0;
			FT_Set_Transform(face, &ScaleMatrix, nullptr);
		}
		else
		{
			FT_Set_Transform(face, nullptr, nullptr);
		}

*/
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfreetypeDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



