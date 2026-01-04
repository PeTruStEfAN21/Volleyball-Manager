cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

# DEPENDINȚE EXTERNE (SFML)
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0
    GIT_SHALLOW    1
    CMAKE_ARGS
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5
        -DSFML_SKIP_MANDATORY_DEPENDENCY_SEARCH=TRUE
        -DSFML_BUILD_EXAMPLES=OFF
        -DSFML_BUILD_TESTS=OFF
        -DSFML_BUILD_NETWORK=ON
        -DSFML_BUILD_GRAPHICS=ON
        -DSFML_BUILD_WINDOW=ON
        -DSFML_BUILD_SYSTEM=ON
        -DSFML_BUILD_AUDIO=ON
)
FetchContent_MakeAvailable(SFML)

find_package(Threads REQUIRED)
if(UNIX AND NOT APPLE)
    find_package(X11)
endif()

# LISTA DE FIȘIERE (PE CARE LE AVEAI ÎNAINTE)
# Le caută în folderul curent, nu în src/
add_executable(${MAIN_EXECUTABLE_NAME}
    src/main.cpp
    src/ListaJucatoriGui.cpp include/ListaJucatoriGui.h
    include/Screen.h
    src/TransferScreen.cpp include/TransferScreen.h
    src/Button.cpp include/Button.h
    src/MainMenuScreen.cpp include/MainMenuScreen.h
    src/InitialDraftScreen.cpp include/InitialDraftScreen.h
    src/FirstSixScreen.cpp include/FirstSixScreen.h
    src/TableScore.cpp include/TableScore.h
    src/MatchScreen.cpp include/MatchScreen.h
    src/TimeoutScreen.cpp include/TimeoutScreen.h
    include/Exceptii.h
    src/LigaScreen.cpp include/LigaScreen.h
    src/ScoreboardScreen.cpp include/ScoreboardScreen.h
    src/MatchLeague.cpp include/MatchLeague.h
    include/manageri.h src/manageri.cpp
    include/BazaDeDate.h src/BazaDeDate.cpp
    include/Liga.h src/Liga.cpp
    include/Echipe.h src/Echipe.cpp
    include/jucator.h src/jucator.cpp
    include/Meci.h src/Meci.cpp
    include/Transferuri.h src/Transferuri.cpp
    src/UsernameScreen.cpp include/UsernameScreen.h
    src/PachetScreen.cpp include/PachetScreen.h
    src/Pachete.cpp include/Pachete.h
    include/HistoryScreen.h
    include/Utilitare.h
    include/Gestiune.h
    include/Strategies.h
)


set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${sfml_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${sfml_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio Threads::Threads)
target_include_directories(${MAIN_EXECUTABLE_NAME}
    PRIVATE
        src
	include
)



if(UNIX AND NOT APPLE)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})