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
    main.cpp
    # generated/src/Helper.cpp # Decomenteaza daca il folosesti
    ListaJucatoriGui.cpp ListaJucatoriGui.h
    Screen.h
    TransferScreen.cpp TransferScreen.h
    Button.cpp Button.h
    MainMenuScreen.cpp MainMenuScreen.h
    InitialDraftScreen.cpp InitialDraftScreen.h
    FirstSixScreen.cpp FirstSixScreen.h
    TableScore.cpp TableScore.h
    MatchScreen.cpp MatchScreen.h
    TimeoutScreen.cpp TimeoutScreen.h
    Exceptii.h
    LigaScreen.cpp LigaScreen.h
    ScoreboardScreen.cpp ScoreboardScreen.h
    MatchLeague.cpp MatchLeague.h
    manageri.h manageri.cpp
    BazaDeDate.h BazaDeDate.cpp
    Liga.h Liga.cpp
    Echipe.h Echipe.cpp
    jucator.h jucator.cpp
    Meci.h Meci.cpp
    Transferuri.h Transferuri.cpp
    UsernameScreen.cpp UsernameScreen.h
    PachetScreen.cpp PachetScreen.h
    Pachete.cpp Pachete.h
    HistoryScreen.h
    Utilitare.h
    Gestiune.h
    Strategies.h
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE generated/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${sfml_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${sfml_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio Threads::Threads)

if(UNIX AND NOT APPLE)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})