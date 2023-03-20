cmake_minimum_required(VERSION 3.21)
project(outcast-particle-app LANGUAGES CXX)

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
add_executable(outcast-particle-app ${SRC_DIR} ${COMPONENTS_DIR} ${SYSTEMS_DIR} ${UTILS_DIR})
target_include_directories(outcast-particle-app PUBLIC src src/components src/systems)

target_link_libraries(outcast-particle-app PRIVATE sfml-graphics sfml-audio)
target_compile_features(outcast-particle-app PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET outcast-particle-app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:outcast-particle-app> $<TARGET_FILE_DIR:outcast-particle-app> COMMAND_EXPAND_LISTS)
endif()
add_custom_command(TARGET outcast-particle-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/build/_deps/sfml-src/extlibs/bin/x64/openal32.dll"
        $<TARGET_FILE_DIR:outcast-particle-app>)

install(TARGETS outcast-particle-app)
