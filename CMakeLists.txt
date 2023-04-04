cmake_minimum_required(VERSION 3.21)
project(volatile-particle-app LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1)
FetchContent_MakeAvailable(SFML)

add_compile_options(/bigobj)

aux_source_directory(src/ SRC_DIR)
aux_source_directory(src/components/ COMPONENTS_DIR)
aux_source_directory(src/systems/ SYSTEMS_DIR)
aux_source_directory(src/utils/ UTILS_DIR)
aux_source_directory(src/ecs/ ECS_DIR)
add_executable(volatile-particle-app ${SRC_DIR} ${COMPONENTS_DIR} ${SYSTEMS_DIR} ${UTILS_DIR} ${ECS_DIR})
set_target_properties(volatile-particle-app PROPERTIES OUTPUT_NAME "Volatile Particle")
target_include_directories(volatile-particle-app PUBLIC src src/components src/systems)

target_link_libraries(volatile-particle-app PRIVATE sfml-graphics sfml-audio)
target_compile_features(volatile-particle-app PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET volatile-particle-app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:volatile-particle-app> $<TARGET_FILE_DIR:volatile-particle-app> COMMAND_EXPAND_LISTS)
endif()
add_custom_command(TARGET volatile-particle-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/build/_deps/sfml-src/extlibs/bin/x64/openal32.dll"
        $<TARGET_FILE_DIR:volatile-particle-app>)

install(TARGETS volatile-particle-app)
