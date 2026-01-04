cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

# 1. Setam standardul C++ (SFML 3 cere minim 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directoare pentru header-e
include_directories(src)
include_directories(include)

# 2. CONFIGURARE SFML (Aici rezolvam eroarea cu MbedTLS)
include(FetchContent)

# Dezactivam modulele de care nu avem nevoie ca sa nu ceara dependinte extra pe Linux
set(SFML_BUILD_NETWORK  OFF CACHE BOOL "" FORCE) # <--- ASTA REZOLVA EROAREA TA!
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

# 3. GASIREA AUTOMATA A FISIERELOR SURSA
# Cauta recursiv orice fisier .cpp si .h in folderul src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Verificare de siguranta
if(NOT SOURCES)
    message(FATAL_ERROR "Nu am gasit niciun fisier sursa in folderul src/! Verifica daca ai mutat fisierele acolo.")
endif()

# Adaugam si helper-ul generat daca exista (specific template-ului tau)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/src/Helper.cpp")
    list(APPEND SOURCES "generated/src/Helper.cpp")
endif()

# 4. CREAREA EXECUTABILULUI
add_executable(${MAIN_EXECUTABLE_NAME} ${SOURCES})

# 5. LINKEAREA BIBLIOTECILOR
# Cautam Thread-uri (necesare pe Linux)
find_package(Threads REQUIRED)

# Legam modulele SFML (Fara Network!)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    Threads::Threads
)

# Pe Linux avem nevoie de X11 explicit
if(UNIX AND NOT APPLE)
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
    endif()
endif()

# 6. INSTALARE SI COPIERE FISIERE (RESURSE)
# Helpers pentru instalare (specifice template-ului de GitHub Actions)
include(cmake/Options.cmake OPTIONAL_RESULT VARIABLE)
include(cmake/CopyHelper.cmake OPTIONAL_RESULT VARIABLE)
include(cmake/CompilerFlags.cmake OPTIONAL_RESULT VARIABLE)

# Setam flag-uri compilator daca exista helperul
if(COMMAND set_compiler_flags)
    set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
endif()

# Instalam tinta
if(DEFINED DESTINATION_DIR)
    install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
endif()

# Copiem tastatura.txt langa executabil (daca exista)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt")
    add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/tastatura.txt"
            "$<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>")
endif()