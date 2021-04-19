set(GLAD_SRC 
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c)

include_directories(include)

add_library(glad STATIC ${GLAD_SRC})