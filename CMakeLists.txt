
cmake_minimum_required(VERSION 3.10)

project(untitled2)
set(MAIN_EXECUTABLE_NAME "untitled2")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)


set(BIN_DIR "bin") 

add_executable(untitled2 main.cpp)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

install(
    TARGETS untitled2
    DESTINATION ${BIN_DIR}  
)

install(
    FILES launcher.command
    DESTINATION ${BIN_DIR} 
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)
