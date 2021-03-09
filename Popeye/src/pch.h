#pragma once

#include <stdint.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <utility>

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

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#ifdef POPEYE_PLATFORM_WINDOWS
#include <windows.h>
#endif // ENGINE_PLATFORM_WINDOWS


