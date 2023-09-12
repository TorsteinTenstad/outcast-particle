cmake_minimum_required(VERSION 3.21)
project(volatile-particle-app LANGUAGES CXX)
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
add_executable(volatile-particle-app WIN32 ${SRC_DIR} ${COMPONENTS_DIR} ${SYSTEMS_DIR} ${UTILS_DIR} ${ECS_DIR} ${EDIT_MODE_ACTIONS_DIR})
set_target_properties(volatile-particle-app PROPERTIES OUTPUT_NAME "Volatile Particle")
target_include_directories(volatile-particle-app PUBLIC src src/components src/systems)

SET(CMAKE_CXX_FLAGS "/EHsc")

target_link_libraries(volatile-particle-app PRIVATE sfml-graphics sfml-audio sfml-main)
target_compile_features(volatile-particle-app PRIVATE cxx_std_17)

add_custom_command(
    TARGET volatile-particle-app
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:volatile-particle-app>
        $<TARGET_FILE_DIR:volatile-particle-app> COMMAND_EXPAND_LISTS
)

add_custom_command(
    TARGET volatile-particle-app
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll
        $<TARGET_FILE_DIR:volatile-particle-app>
)

add_custom_command(
    TARGET volatile-particle-app
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/game_data
        $<TARGET_FILE_DIR:volatile-particle-app>/game_dat
)

install(TARGETS volatile-particle-app)
