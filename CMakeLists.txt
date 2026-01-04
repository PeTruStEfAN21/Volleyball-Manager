cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directoare
include_directories(src)
include_directories(include)

# -----------------------------------------------------------------------------
# Configurare SFML (Fără Network pentru a evita eroarea MbedTLS)
# -----------------------------------------------------------------------------
include(FetchContent)

# Oprim Network (Internet) ca sa nu ceara criptare (MbedTLS) pe Linux
set(SFML_BUILD_NETWORK  OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        master
    GIT_SHALLOW    1
)
FetchContent_MakeAvailable(SFML)

# -----------------------------------------------------------------------------
# SURSELE (AICI E FIXUL: Cauta automat in folderul src/)
# -----------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

if(NOT SOURCES)
    message(FATAL_ERROR "Nu am gasit fisiere in folderul src/! Verifica structura.")
endif()

# Adăugăm helper-ul generat de template dacă există
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/src/Helper.cpp")
    list(APPEND SOURCES "generated/src/Helper.cpp")
endif()

# -----------------------------------------------------------------------------
# Executabil și Linkare
# -----------------------------------------------------------------------------
add_executable(${MAIN_EXECUTABLE_NAME} ${SOURCES})

find_package(Threads REQUIRED)

target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    Threads::Threads
)

if(UNIX AND NOT APPLE)
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
    endif()
endif()

# -----------------------------------------------------------------------------
# Copiere Resurse
# -----------------------------------------------------------------------------
include(cmake/CopyHelper.cmake OPTIONAL_RESULT VARIABLE)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt")
    add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt"
            "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>")
endif()

# Activează sanitizers
include(cmake/CompilerFlags.cmake OPTIONAL_RESULT VARIABLE)
if(COMMAND set_compiler_flags)
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
endif()

if(DEFINED DESTINATION_DIR)
    install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
endif()