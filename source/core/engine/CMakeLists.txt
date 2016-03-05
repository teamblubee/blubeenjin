SET(CMAKE_MACOSX_RPATH 1)

SET(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/headers/core_engine.h)
SET(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/core_engine.c)

SET(GLAD_HEADERS "${CMAKE_SOURCE_DIR}/source/deps/glad/headers")
SET(GLFW_HEADERS "${CMAKE_SOURCE_DIR}/source/deps/glfw-3.1.2/include")
SET(SCALAR_MATH_HEADERS "${CMAKE_SOURCE_DIR}/source/core/math/scalar")


SET(TARGET_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers" "${GLAD_HEADERS}"
    "${GLFW_HEADERS}" "${SCALAR_MATH_HEADERS}")

SET(TARGET_LIBS glfw glad core_math resource_utils vertex_layout  mesh sprite shader image texture)

ADD_LIBRARY(core_engine SHARED ${SRC_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(core_engine ${TARGET_LIBS})
TARGET_INCLUDE_DIRECTORIES(core_engine PUBLIC ${TARGET_INC_DIR})
