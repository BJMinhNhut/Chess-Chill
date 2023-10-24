cmake_minimum_required(VERSION 3.16)
project(Chess LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(src)
add_executable(Chess src/main.cpp
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
        src/Template/SoundPlayer.cpp)
target_link_libraries(Chess PRIVATE sfml-graphics)
target_compile_features(Chess PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET Chess
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Chess>
        VERBATIM)
endif()

install(TARGETS Chess)
