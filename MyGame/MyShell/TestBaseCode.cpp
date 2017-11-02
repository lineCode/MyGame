#include "stdafx.h"
#include "TestBaseCode.h"
#include "BaseFunc.h"
#include "MyThread.h"

#include "imgui/imgui.h"
#include  "imgui_impl_Render.h"

extern LRESULT ImGui_ImplC3_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class CThreadEventAutoFindRoad : public IThreadEvent
{
public:
	virtual ~CThreadEventAutoFindRoad() {}
protected:
	virtual void OnThreadCreate(void)
	{
		g_objTestBaseCode.OnThreadBeginAutoFindRoad();
	}
};

TestBaseCode& TestBaseCode::GetInstance()
{
	static TestBaseCode _instance;
	return _instance;
}

TestBaseCode* TestBaseCode::GetInstancePtr()
{
	TestBaseCode& _instance = GetInstance();
	return &_instance;
}

TestBaseCode::TestBaseCode()
{
	m_pThreadAutoFindRoad = NULL;
	m_pThreadEventAutoFindRoad = NULL;
}


TestBaseCode::~TestBaseCode()
{
	Release();
}

void TestBaseCode::Release()
{
	SAFE_DELETE(m_pThreadAutoFindRoad);
	SAFE_DELETE(m_pThreadEventAutoFindRoad);
}

void TestBaseCode::RunTest()
{	
	ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();

	ImFont*   pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChinese());

	ImGui::GetIO().FontDefault = pFont;
	BeginAutoFindRoad();
	EndAutoFindRoad();
}

void TestBaseCode::OnThreadBeginAutoFindRoad()
{
	while (!mTimerUpdate.IsTimeOut())
	{
		DebugMyMsg("%d", mTimerUpdate.GetPassTime());
	}

	DebugMyMsg("开始加载字体");
	m_lockFindRoad.Enter();
	unsigned char* pixels;
	int width, height;
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	DebugMyMsg("字体大小：%d %d", width, height);
	DebugMyMsg("完成加载字体");
	DebugMyMsg("%d", mTimerUpdate.GetPassTime());
	m_lockFindRoad.Leave();
}

bool TestBaseCode::BeginAutoFindRoad()
{
	mTimerUpdate.Startup( 1000);
	if (NULL == m_pThreadEventAutoFindRoad)
	{
		m_pThreadEventAutoFindRoad = new CThreadEventAutoFindRoad();
	}
	m_pThreadAutoFindRoad = CMyThread::CreateNew(*m_pThreadEventAutoFindRoad, CMyThread::RUN, 0);

	return true;
}

void TestBaseCode::EndAutoFindRoad()
{
	if (NULL != m_pThreadAutoFindRoad)
	{
		m_pThreadAutoFindRoad->EndThread();
		SAFE_DELETE(m_pThreadAutoFindRoad);
	}
}
