SET(CMAKE_MACOSX_RPATH 1)

SET(SRC_FILES main.c)
SET(CMAKE_SHARED_LINKER_FLAGS "-g")

use_c99()

ADD_EXECUTABLE(main ${SRC_FILES})
TARGET_LINK_LIBRARIES(main core_engine)



