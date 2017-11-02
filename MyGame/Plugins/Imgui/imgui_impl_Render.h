#pragma once
#include "imgui/imgui.h"


IMGUI_API bool        ImGui_ImplRender_Init(void* hwnd);
IMGUI_API void        ImGui_ImplRender_Shutdown();
IMGUI_API void        ImGui_ImplRender_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplRender_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplRender_CreateDeviceObjects();