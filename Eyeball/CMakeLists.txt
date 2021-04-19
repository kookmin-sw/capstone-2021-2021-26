if(WIN32)
    add_definitions(-DPOPEYE_PLATFORM_WINDOWS)
endif()

set(APP_SRC 
    src/App.cpp)

add_executable(eyeball 
    ${APP_SRC})

target_link_libraries(eyeball
    PUBLIC popeye)

include_directories(../Popeye/src
    ../Popeye/vendor/spdlog/include)

set_target_properties(eyeball PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin/PopeyeEngine)

target_compile_features(eyeball PUBLIC cxx_std_17)