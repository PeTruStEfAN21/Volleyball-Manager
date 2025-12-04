cmake_minimum_required(VERSION 3.10)
project(oop)

# SFML 3 cere C++17 minim.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ====================================================================
# PASUL 1: CONFIGURARE SFML 3.0.0 (FETCHCONTENT)
# ====================================================================
# Aici cerem explicit versiunea 3.
find_package(SFML 3 QUIET COMPONENTS Graphics Window System Audio)

if(NOT SFML_FOUND)
    message(STATUS "SFML 3 not found. Fetching SFML 3.0.0 from GitHub...")
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)
    # set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    FetchContent_Declare(
            sfml
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            # AICI E VERSUNEA PE CARE O VREI TU:
            GIT_TAG        3.0.0
            GIT_SHALLOW    1
    )
    # SFML 3 necesită uneori setarea asta pentru a construi exemplele/testele dacă nu le vrei
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TESTS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(sfml)
    message(STATUS "SFML 3.0.0 fetch done.")
endif()

# ====================================================================
# PASUL 2: DEFINIȚII FIȘIERE SURSĂ
# ====================================================================

# PO CORE FILES (Logica Jocului)
set(PO_BASE_FILES
        jucator.cpp jucator.h
        Echipe.cpp Echipe.h
        BazaDeDate.cpp BazaDeDate.h
        Meci.cpp
        Liga.cpp Liga.h
        manageri.cpp manageri.h
        Turneu.cpp Turneu.h
        Transferuri.cpp Transferuri.h
)

# GUI FILES (SFML Dependent)
set(GUI_SOURCE_FILES
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
        LigaScreen.cpp
        LigaScreen.h
        ScoreboardScreen.cpp
        ScoreboardScreen.h
        MatchLeague.cpp
        MatchLeague.h
)

# ====================================================================
# PASUL 3: DEFINIREA EXECUTABILELOR
# ====================================================================

# --- 1. TARGETUL GUI (VoleiManager_GUI) ---
add_executable(VoleiManager_GUI
        main.cpp
        ${PO_BASE_FILES}
        ${GUI_SOURCE_FILES}
)

# Linkarea pentru SFML 3 e la fel
target_link_libraries(VoleiManager_GUI
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

# --- FIXUL SUPREM PENTRU DLL-uri PE WINDOWS (Functioneaza si la SFML 3) ---
if(WIN32 AND TARGET SFML::Graphics)
    add_custom_command(TARGET VoleiManager_GUI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Graphics>
            $<TARGET_FILE:SFML::Window>
            $<TARGET_FILE:SFML::System>
            $<TARGET_FILE:SFML::Audio>
            # SFML 3 poate avea nevoie si de alte DLL-uri externe (ex: freetype)
            # CMake ar trebui sa le gestioneze, dar daca lipsesc, trebuie adaugate aici.
            $<TARGET_FILE_DIR:VoleiManager_GUI>
            COMMENT "Copiere DLL-uri SFML 3 langa executabil..."
    )
endif()

# --- FIXUL PENTRU RESURSE (Working Directory) ---
set_target_properties(VoleiManager_GUI PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# --- 2. TARGETUL CONSOLĂ (VoleiManager_Console) ---
add_executable(VoleiManager_Console
        main_console.cpp
        ${PO_BASE_FILES}
)

target_link_libraries(VoleiManager_Console
        PRIVATE
        SFML::System
)