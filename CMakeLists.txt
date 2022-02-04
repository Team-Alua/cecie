cmake_minimum_required(VERSION 3.0)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cecie)

SET(TOOLCHAIN $ENV{OO_PS4_TOOLCHAIN})

SET(CMAKE_C_COMPILER /usr/bin/clang)

SET(CMAKE_CXX_COMPILER /usr/bin/clang++)

SET(EXEC_NAME "${CMAKE_PROJECT_NAME}.elf")

SET(OBJ_DIR "${CMAKE_SOURCE_DIR}${CMAKE_FILES_DIRECTORY}/${EXEC_NAME}.dir/source")

SET(LIBS "-lc -lkernel -lc++")

include_directories(BEFORE SYSTEM $ENV{OO_PS4_TOOLCHAIN}/include)
include_directories(${root_SOURCE_DIR}/include)

set(SOURCES source/main.cpp)

add_executable(${EXEC_NAME} ${SOURCES})
SET(OBJ_FILES 
    ${OBJ_DIR}/main.cpp.o
)
SET(LD_INIT_FLAGS "${OBJ_FILES} -o ${EXEC_NAME} ")
SET(LD_FLAGS "-v -m elf_x86_64 -pie --script ${TOOLCHAIN}/link.x --eh-frame-hdr -L${TOOLCHAIN}/lib ${LIBS} ${TOOLCHAIN}/lib/crt1.o")
SET(CMAKE_C_LINK_EXECUTABLE "/usr/bin/ld.lld ${LD_INIT_FLAGS} ${LD_FLAGS}")
SET(CMAKE_CXX_LINK_EXECUTABLE "/usr/bin/ld.lld ${LD_INIT_FLAGS} ${LD_FLAGS}")
