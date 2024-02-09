cmake_minimum_required(VERSION  3.16)
project(fluid LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG  2.6.x)
FetchContent_GetProperties(SFML)

if(NOT sfml_POPULATED)
    FetchContent_Populate(SFML)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

add_executable(fluid src/main.cpp)
target_include_directories(fluid PRIVATE headers)
target_link_libraries(fluid PRIVATE sfml-graphics sfml-audio)
target_compile_features(fluid PRIVATE cxx_std_23)

if(WIN32)
    add_custom_command(
        TARGET fluid
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:fluid>
        VERBATIM)
endif()

install(TARGETS fluid)