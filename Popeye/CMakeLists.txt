
if(WIN32)
    add_definitions(-DPOPEYE_PLATFORM_WINDOWS)
    set(PLATFORM_HEADER <windows.h>)
endif()
add_definitions(-DPOPEYE_BUILD_DLL)

set(POPEYE_SRC
    src/Popeye.h
    src/Popeye/Application.h
    src/Popeye/Application.cpp
    src/Popeye/Core.h
    src/Popeye/EntryPoint.h
    src/Popeye/Log.h
    src/Popeye/Log.cpp
    src/Popeye/Mainframe.h
    src/Popeye/Mainframe.cpp
    src/Popeye/stb_img.cpp
)

set(POPEYE_COMPONENT 
    src/Popeye/Component/RenderingComponents.h
    src/Popeye/Component/RenderingComponents.cpp)

set(POPEYE_EVENT 
    src/Popeye/Events/EventManager.h    
    src/Popeye/Events/EventManager.cpp
    src/Popeye/Events/KeyCode.h
    src/Popeye/Events/MouseCode.h
    src/Popeye/Events/Events.h)

set(POPEYE_GUI 
    src/Popeye/GUI/IconsForkAwesome.h
    src/Popeye/GUI/imgui_impl_glfw.h
    src/Popeye/GUI/imgui_impl_glfw.cpp
    src/Popeye/GUI/imgui_impl_opengl3.h
    src/Popeye/GUI/imgui_impl_opengl3.cpp
    src/Popeye/GUI/imgui_custom_widgets.h
    src/Popeye/GUI/imgui_custom_widgets.cpp
    src/Popeye/GUI/Tabs.h
    src/Popeye/GUI/Tabs.cpp)

set(POPEYE_MANAGER
    src/Popeye/Manager/GUIManager.h
    src/Popeye/Manager/GUIManager.cpp
    src/Popeye/Manager/FileManager.h
    src/Popeye/Manager/FileManager.cpp
    src/Popeye/Manager/ResourceManager.h
    src/Popeye/Manager/ResourceManager.cpp
    src/Popeye/Manager/SceneManager.h
    src/Popeye/Manager/SceneManager.cpp
    src/Popeye/Manager/ComponentManager.h
    src/Popeye/Manager/ComponentManager.cpp)

set(POPEYE_RESOURCE 
    src/Popeye/Resource/Texture.h)

set(POPEYE_SCENE 
    src/Popeye/Scene/GameObject.h
    src/Popeye/Scene/GameObject.cpp
    src/Popeye/Scene/Scene.h
    src/Popeye/Scene/Scene.cpp)

set(POPEYE_SYSTEM 
    src/Popeye/System/EventSystem.h
    src/Popeye/System/EventSystem.cpp
    src/Popeye/System/RenderingSystem.h
    src/Popeye/System/RenderingSystem.cpp)

add_library(popeye SHARED
    ${POPEYE_SRC}
    ${POPEYE_COMPONENT}
    ${POPEYE_EVENT}
    ${POPEYE_GUI}
    ${POPEYE_MANAGER}
    ${POPEYE_RESOURCE}
    ${POPEYE_SCENE}
    ${POPEYE_SYSTEM})

add_subdirectory(vendor/glm)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/imgui)

include_directories(popeye 
    vendor/GLFW/include
    vendor/glad/include
    vendor/spdlog/include
    vendor/imgui
    vendor/glm
    vendor/stb_image
    )

target_link_libraries(popeye 
    PUBLIC 
    glad
    glfw
    imgui
    opengl32.lib)

target_precompile_headers(popeye PRIVATE
    ${PLATFORM_HEADER}
    
    <stdint.h>

    <filesystem>
    <iostream>
    <fstream>
    <sstream>
    <memory>
    <string>
    <vector>
    <queue>
    <map>
    <unordered_map>
    <algorithm>
    <utility>

    src/Popeye/Log.h

    <glad/glad.h>
    <GLFW/glfw3.h>

    <glm/glm.hpp>
    <glm/gtc/matrix_transform.hpp>
    <glm/gtc/type_ptr.hpp>
    <glm/gtx/quaternion.hpp>
    
    <imgui.h>
    <imgui_internal.h>
    )

set_target_properties(popeye PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin/PopeyeEngine)

target_compile_features(popeye PUBLIC cxx_std_17)