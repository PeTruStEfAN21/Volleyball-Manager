cmake_minimum_required(VERSION 3.10)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled2 main.cpp)

install(
    TARGETS untitled2  
    DESTINATION bin    
)

install(
    FILES launcher.command
    DESTINATION ${BIN_DIR}  # presupunând că ${BIN_DIR} este 'bin'
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)