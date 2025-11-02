
cmake_minimum_required(VERSION 3.10)

project(oop)

set(CMAKE_CXX_STANDARD 20)


set(BIN_DIR "bin") 

add_executable(oop main.cpp)


install(
    TARGETS oop
    DESTINATION ${BIN_DIR}  
)

install(
    FILES launcher.command
    DESTINATION ${BIN_DIR} 
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)
