cmake_minimum_required(VERSION 3.16)
set(TARGET_NAME Chess-Chill)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(${TARGET_NAME} LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

if (WIN32)
    enable_language("RC")
    set (WIN32_RESOURCES src/icon.rc)
endif()

include_directories(src)
add_executable(${TARGET_NAME}
        ${WIN32_RESOURCES}
        src/main.cpp
        src/Template/Application.cpp
        src/States/StatesList.hpp
        src/States/GameState.cpp
        src/States/GameState.hpp
        src/Game/GameHandler.cpp
        src/Game/GameHandler.hpp
        src/Template/SpriteNode.cpp
        src/Template/SpriteNode.hpp
        src/Template/SceneNode.cpp
        src/Template/State.cpp
        src/Template/StateStack.cpp
        src/Template/Utility.cpp
        src/Game/Piece.cpp
        src/Game/Piece.hpp
        src/Template/RectNode.cpp
        src/Template/RectNode.hpp
        src/Template/SoundPlayer.hpp
        src/Template/SoundPlayer.cpp
        src/Game/GameLogic.cpp
        src/Game/GameLogic.hpp
        src/Game/AttackGenerator.cpp
        src/Game/AttackGenerator.hpp
        src/Game/Board.cpp
        src/Game/Board.hpp
        src/States/MenuState.cpp
        src/GUI/Component.cpp
        src/GUI/Container.cpp
        src/GUI/Button.cpp
        src/States/AboutState.cpp
        src/States/AboutState.hpp
        src/GUI/Label.cpp
        src/GUI/Sprite.cpp
        src/States/SettingsState.cpp
        src/States/SettingsState.hpp
        src/Template/Settings.cpp
        src/Game/AttackBoard.cpp
        src/Game/AttackBoard.hpp
        src/Game/Clock.cpp
        src/Game/Clock.hpp
        src/States/GameOptionsState.cpp
        src/States/GameOptionsState.hpp
        src/Template/GameOptions.cpp
        src/Template/GameOptions.hpp
        src/Game/Player.cpp
        src/Game/Player.hpp
        src/Game/HumanPlayer.cpp
        src/Game/HumanPlayer.hpp
        src/Game/BotPlayer.cpp
        src/Game/BotPlayer.hpp
        src/Template/Random.cpp
)
target_link_libraries(${TARGET_NAME} PRIVATE sfml-graphics sfml-audio)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${TARGET_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${TARGET_NAME}>
        VERBATIM)
endif()

install(TARGETS ${TARGET_NAME})
