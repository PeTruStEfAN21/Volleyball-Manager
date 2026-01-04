cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

# Setam standardul C++ (SFML 3 cere minim 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directoare
include_directories(src)
include_directories(include)

# ---------------------------------------------------------
# 1. DEPENDINTE EXTERNE (SFML)
# ---------------------------------------------------------
include(FetchContent)

# Setam optiunile SFML inainte sa il descarcam
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        master  # Folosim master pt SFML 3
    GIT_SHALLOW    1
)
FetchContent_MakeAvailable(SFML)

# ---------------------------------------------------------
# 2. GASIREA FISIERELOR SURSA (FIX-UL TAU)
# ---------------------------------------------------------
# Comanda asta cauta recursiv orice fisier .cpp si .h in folderul src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Daca nu gaseste nimic, da eroare (ca sa stii sigur)
if(NOT SOURCES)
    message(FATAL_ERROR "Nu am gasit niciun fisier sursa in folderul src/ !")
endif()

# Adaugam si helper-ul generat daca exista
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/src/Helper.cpp")
    list(APPEND SOURCES "generated/src/Helper.cpp")
endif()

# ---------------------------------------------------------
# 3. CREAREA EXECUTABILULUI
# ---------------------------------------------------------
add_executable(${MAIN_EXECUTABLE_NAME} ${SOURCES})

# ---------------------------------------------------------
# 4. LINK-AREA BIBLIOTECILOR
# ---------------------------------------------------------
# Cautam dependinte de sistem pentru Linux
find_package(Threads REQUIRED)

target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    Threads::Threads
)

# Pe Linux avem nevoie de X11 explicit uneori
if(UNIX AND NOT APPLE)
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
    endif()
endif()

# ---------------------------------------------------------
# 5. INSTALARE SI COPIERE RESURSE
# ---------------------------------------------------------
# (Pastram ce aveai tu pentru copierea tastatura.txt)
include(cmake/CopyHelper.cmake OPTIONAL_RESULT VARIABLE) 
# Daca ai fisierul CopyHelper, il folosim, daca nu, folosim comanda standard:

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt")
    add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt"
            "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>")
endif()