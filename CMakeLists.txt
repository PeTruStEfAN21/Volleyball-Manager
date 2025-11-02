cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME "oop")
project(${PROJECT_NAME} CXX) # Adăugat CXX pentru a specifica limbajul

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp)


target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:-Wall -Wextra -Werror>
)

target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Release>:-O3>
)
set(BIN_DIR "bin") 

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${BIN_DIR}
)

if(APPLE)
    install(
        FILES launcher.command
        DESTINATION ${BIN_DIR}
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_READ GROUP_EXECUTE
                      WORLD_READ WORLD_EXECUTE
    )
endif()
