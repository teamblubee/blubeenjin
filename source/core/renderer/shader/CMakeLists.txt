SET(CMAKE_MACOSX_RPATH 1)

SET(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/headers/shader.h)
SET(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shader.c)

SET(GLAD_HEADERS "${CMAKE_SOURCE_DIR}/source/deps/glad/headers")
SET(TARGET_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers" ${GLAD_HEADERS})

SET(TARGET_LIBS resource_utils)

use_c99()

ADD_LIBRARY(shader SHARED ${SRC_FILES} ${HEADER_FILES})
TARGET_LINK_LIBRARIES(shader ${TARGET_LIBS})
TARGET_INCLUDE_DIRECTORIES(shader PUBLIC ${TARGET_INC_DIR})
