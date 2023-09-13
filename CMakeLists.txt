cmake_minimum_required(VERSION 3.21)
project(volatile-particle-app LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)
set(SFML_USE_STATIC_STD_LIBS TRUE)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1)
FetchContent_MakeAvailable(SFML)

aux_source_directory(src/ SRC_DIR)
aux_source_directory(src/components/ COMPONENTS_DIR)
aux_source_directory(src/systems/ SYSTEMS_DIR)
aux_source_directory(src/utils/ UTILS_DIR)
aux_source_directory(src/ecs/ ECS_DIR)
aux_source_directory(src/edit_mode_actions/ EDIT_MODE_ACTIONS_DIR)
add_executable(volatile-particle-app ${_WIN32} ${SRC_DIR} ${COMPONENTS_DIR} ${SYSTEMS_DIR} ${UTILS_DIR} ${ECS_DIR} ${EDIT_MODE_ACTIONS_DIR})

set_target_properties(volatile-particle-app PROPERTIES OUTPUT_NAME "Volatile Particle")
target_include_directories(volatile-particle-app PUBLIC src src/components src/systems)

SET(CMAKE_CXX_FLAGS "/EHsc")

target_link_libraries(volatile-particle-app PRIVATE sfml-graphics sfml-audio sfml-main)
target_compile_features(volatile-particle-app PRIVATE cxx_std_17)

add_custom_command(
    TARGET volatile-particle-app
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/extlibs/bin/x64/openal32.dll
        $<TARGET_FILE_DIR:volatile-particle-app>
)

add_custom_command(
    TARGET volatile-particle-app
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/game_data
        $<TARGET_FILE_DIR:volatile-particle-app>/game_data
)

install(TARGETS volatile-particle-app)
