#include "imgui/imgui.h"
#include "imgui_impl_Render.h"
#include "PlatformInterface.h"

bool ImGui_ImplRender_Init(void* hwnd)
{
	return true;
}

 void ImGui_ImplRender_Shutdown()
{

}

 void ImGui_ImplRender_NewFrame()
{

}

 void ImGui_ImplRender_InvalidateDeviceObjects()
{

}

 bool ImGui_ImplRender_CreateDeviceObjects()
{
	 return true;
}

 IMGUI_API LRESULT ImGui_ImplRender_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
 {
	 ImGuiIO& io = ImGui::GetIO();
	 switch (msg)
	 {
	 case WM_LBUTTONDOWN:
		 io.MouseDown[0] = true;
		 return true;
	 case WM_LBUTTONUP:
		 io.MouseDown[0] = false;
		 return true;
	 case WM_RBUTTONDOWN:
		 io.MouseDown[1] = true;
		 return true;
	 case WM_RBUTTONUP:
		 io.MouseDown[1] = false;
		 return true;
	 case WM_MBUTTONDOWN:
		 io.MouseDown[2] = true;
		 return true;
	 case WM_MBUTTONUP:
		 io.MouseDown[2] = false;
		 return true;
	 case WM_MOUSEWHEEL:
		 io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		 return true;
	 case WM_MOUSEMOVE:
		 io.MousePos.x = (signed short)(lParam);
		 io.MousePos.y = (signed short)(lParam >> 16);
		 return true;
	 case WM_KEYDOWN:
		 if (wParam < 256)
			 io.KeysDown[wParam] = 1;
		 return true;
	 case WM_KEYUP:
		 if (wParam < 256)
			 io.KeysDown[wParam] = 0;
		 return true;
	 case WM_CHAR:
		 // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		 if (wParam > 0 && wParam < 0x10000)
			 io.AddInputCharacter((unsigned short)wParam);
		 return true;
	 }
	 return 0;
 }
