#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include "Popeye/Log.h"

//opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//imgui
#include "imgui.h"
#include "imgui_internal.h"
#include "Popeye/GUI/imgui_impl_glfw.h"
#include "Popeye/GUI/imgui_impl_opengl3.h"
#include "Popeye/GUI/IconsForkAwesome.h"

#ifdef POPEYE_PLATFORM_WINDOWS
#include <windows.h>
#endif // ENGINE_PLATFORM_WINDOWS


