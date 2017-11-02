// MyGame.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "resource.h"
#include "GameGraphic.h"
#include "PlatFormInterface.h "
#include "TestBaseCode.h"
#include "MySignal.h"
#include "GameMain.h"
#include "BaseFunc.h"


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


using namespace  XPG;

// this signal stuff is pretty old, not sure if it should be updated 
class vaApplicationSignals
{
public:
	Signal0< void >                   event_Started;
	Signal0< void >                   event_Stopped;
	Signal0< void >                   event_BeforeStopped;
	Signal0< void >                   event_MouseCaptureChanged;

	Signal0< void >                   event_BeforeWindowResized;              // width, height, fullscreen
	Signal3< int, int, bool >         event_WindowResized;                    // width, height, fullscreen

	Signal1< float >                  event_Tick;
	Signal0< void >                   event_Render;

	//static LRESULT CALLBACK WndProcStatic( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	typedef LRESULT & LRESULTREF;
	typedef bool & boolRef;
	Signal6< boolRef, LRESULTREF, HWND, UINT, WPARAM, LPARAM >
		event_WinProcOverride;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	TestPlatformGlobalFunction();

	vaApplicationSignals a;
	a.event_Render.Connect(TestBaseCode::GetInstancePtr(),&TestBaseCode::RunTest);
	a.event_Render.Disconnect(TestBaseCode::GetInstancePtr(), &TestBaseCode::RunTest);

	a.event_Render();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	DWORD dwTime = ::GetTickCount();
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			g_gameMain.ProcessGame();
			UINT uiUse = ::GetTickCount() - dwTime;
			if (uiUse < 20)
			{
				::Sleep(20 - uiUse);
			}
			dwTime = ::GetTickCount();
		}
	}
	g_gameMain.DestoryGameMain();
	UnregisterClass(szWindowClass, hInstance);
	return 0;
/*
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGAME));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;*/
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int WINDOW_WIDTH = DEFAULT_SCREEN_WIDTH;
	int WINDOW_HEIGHT = DEFAULT_SCREEN_HEIGHT;



   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   DEVMODE CurrentMode;
   EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &CurrentMode);
   int x = (CurrentMode.dmPelsWidth - WINDOW_WIDTH) / 2;
   int y = (CurrentMode.dmPelsHeight - WINDOW_HEIGHT) / 2;

  /* HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
	   x, y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   RECT rect;
   ::GetClientRect(hWnd, &rect);

   RECT rectMain;
   GetWindowRect(hWnd, &rectMain);
   int nWidthJudge = WINDOW_WIDTH + (WINDOW_WIDTH - rect.right);
   int nHeightJudge = WINDOW_HEIGHT + (WINDOW_HEIGHT - rect.bottom);
   ::MoveWindow(hWnd, x, y, nWidthJudge, nHeightJudge, TRUE);
   ::GetClientRect(hWnd, &rect);
   GetWindowRect(hWnd, &rectMain);
   if (!g_gameMain.InitGameMain(hInst, hWnd, WINDOW_WIDTH, WINDOW_HEIGHT, NULL))
   {
	   LogMyMsg("g_gameMain.InitGameMain error");
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
const int WINDOWS_MOUSE_CLICK_INDEX = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_gameMain.WndProc(hWnd, message, wParam, lParam);
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		g_objTestBaseCode.Release();
        break;
	case WM_MOUSEWHEEL:
		g_gameMain.OnMouseWheel((int)(short)HIWORD(wParam));
		break;
	case WM_LBUTTONDBLCLK:
		g_gameMain.OnLButtonDblClk((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		g_gameMain.OnLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), WINDOWS_MOUSE_CLICK_INDEX);
		break;
	case WM_LBUTTONUP:
		g_gameMain.OnLButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), WINDOWS_MOUSE_CLICK_INDEX);
		break;
	case WM_RBUTTONUP:
		g_gameMain.OnRButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		g_gameMain.OnMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), WINDOWS_MOUSE_CLICK_INDEX);
		break;
	case WM_KEYDOWN:
		g_gameMain.OnKeyDown(wParam);
		break;
	case WM_KEYUP:
		g_gameMain.OnKeyUp(wParam);
		break;
	case WM_ACTIVATE:
		g_gameMain.OnActivate(LOWORD(wParam), HIWORD(wParam) > 0);
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
