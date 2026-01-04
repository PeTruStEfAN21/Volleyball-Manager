cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

# C++ Standard
set(CMAKE_CXX_STANDARD 17) # SFML 3 cere minim 17. 23 e cam nou, dar poti lasa 20 sau 23.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include helpers
include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

# Unde cautam headere (.h)
include_directories(src)
include_directories(include)

###############################################################################
# SFML FETCH CONTENT
###############################################################################
include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Oprim warning-urile din biblioteci externe
set(WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        master  # Folosim master pentru SFML 3 pana iese tag-ul oficial
    GIT_SHALLOW    1
    
    CMAKE_ARGS
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5
        -DSFML_SKIP_MANDATORY_DEPENDENCY_SEARCH=TRUE
        -DSFML_BUILD_EXAMPLES=OFF
        -DSFML_BUILD_TESTS=OFF
        -DSFML_BUILD_NETWORK=OFF
        -DSFML_BUILD_GRAPHICS=ON
        -DSFML_BUILD_WINDOW=ON
        -DSFML_BUILD_SYSTEM=ON
        -DSFML_BUILD_AUDIO=ON
)

FetchContent_MakeAvailable(SFML)

###############################################################################
# FIND PACKAGES (Linux/Mac)
###############################################################################
find_package(Threads REQUIRED)
if(UNIX AND NOT APPLE)
    find_package(X11)
endif()

###############################################################################
# EXECUTABLE
###############################################################################

# AICI ESTE FIX-UL MAJOR:
# In loc sa scriem manual src/Main.cpp, src/Liga.cpp etc., 
# ii spunem sa ia TOT ce gaseste in src/
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Adaugam si fisierul generat daca e in alt folder
list(APPEND SOURCES "generated/src/Helper.cpp")

add_executable(${MAIN_EXECUTABLE_NAME} ${SOURCES})

# Optiuni compilator
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

###############################################################################
# INCLUDES & LINKING
###############################################################################

# Include directoare
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${sfml_SOURCE_DIR}/include)

# Link Libraries
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    Threads::Threads
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

###############################################################################
# INSTALL & COPY FILES
###############################################################################

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

# Copiem tastatura.txt langa executabil
copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
# Decomenteaza liniile de mai jos daca ai imagini sau sunete
# copy_files(DIRECTORY images sounds COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})