cmake_minimum_required(VERSION 3.16)

project(rtype)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# Enable flags for Debug and Release modes
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Define platform-specific flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")  # Optimize for speed in Release mode for MSVC
endif()

# Define _WIN32_WINNT for Windows builds
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)  # 0x0601 corresponds to Windows 7 and above
endif()

# Suppress unsafe function warnings on Windows
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Use vcpkg for dependencies
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Boost configuration
find_package(Boost REQUIRED COMPONENTS system filesystem date_time serialization)
include_directories(${Boost_INCLUDE_DIRS})

# ASIO configuration
find_package(ASIO REQUIRED)
include_directories(${ASIO_INCLUDE_DIR})

# SFML configuration
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})

# Threads configuration
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Add the directory containing protocol.hpp
include_directories(${CMAKE_SOURCE_DIR}/include)

# Automatically include all header directories by recursively finding all headers
file(GLOB_RECURSE HEADER_FILES "include/**/*.hpp" "include/**/*.h")

# Extract the unique directories from the header files
set(HEADER_DIRS "")
foreach(HEADER_FILE ${HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    list(APPEND HEADER_DIRS ${HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES HEADER_DIRS)

# Add include directories to the project
include_directories(${HEADER_DIRS})

# Output directories
set(CLIENT_BUILD_DIR "${CMAKE_BINARY_DIR}/bin/")
set(SERVER_BUILD_DIR "${CMAKE_BINARY_DIR}/bin/")
file(MAKE_DIRECTORY "${CLIENT_BUILD_DIR}")
file(MAKE_DIRECTORY "${SERVER_BUILD_DIR}")

# Search for all .cpp files in client and gameengine directories
file(GLOB CLIENT_SRC "client/*.cpp")
file(GLOB GAMEENGINE_SRC "gameengine/*.cpp")
file(GLOB GAMEENGINEWINDOW_SRC "client/window/*.cpp")
file(GLOB GAMEENGINEENTITY_SRC "gameengine/entity/*.cpp")
file(GLOB GAMEENGINESYSTEMS_SRC "gameengine/systems/*.cpp")
file(GLOB GAMEENGINEECS_SRC "gameengine/ecs/*.cpp")
file(GLOB GAMEENGINEWINDOWGAME_SRC "client/window/game/*.cpp")
file(GLOB SERVER_SRC "server/*.cpp" "gameengine/ecs/entitymanager.cpp")
file(GLOB CLIENT_MINIGAME_SRC "client/minigame/*.cpp")

# Create the client executable and link all found sources
add_executable(r-type_client
    ${CLIENT_SRC}
    ${GAMEENGINE_SRC}
    ${GAMEENGINEWINDOW_SRC}
    ${GAMEENGINEENTITY_SRC}
    ${GAMEENGINESYSTEMS_SRC}
    ${GAMEENGINEECS_SRC}
    ${GAMEENGINEWINDOWGAME_SRC}
    ${CLIENT_MINIGAME_SRC}
)

# Set C++17 for r-type_client
target_compile_features(r-type_client PUBLIC cxx_std_17)

target_link_libraries(r-type_client
    Threads::Threads
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    Boost::serialization
    asio
)

# rtype Server
include_directories(${CMAKE_INCLUDE_PATH} include/server)

add_executable(r-type_server
    ${SERVER_SRC}
)

# Set C++17 for r-type_server
target_compile_features(r-type_server PUBLIC cxx_std_17)

target_link_libraries(r-type_server
    Threads::Threads
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    Boost::serialization
    asio
)

# Link CoreAudio and AudioToolbox on macOS for OpenAL
if(APPLE)
    target_link_libraries(r-type_client "-framework AudioToolbox" "-framework CoreAudio")
    target_link_libraries(r-type_server "-framework AudioToolbox" "-framework CoreAudio")
endif()

# Link pthread for Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(r-type_server pthread)
endif()

# Output properties for server
set_target_properties(r-type_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${SERVER_BUILD_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${SERVER_BUILD_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${SERVER_BUILD_DIR}"
)

set_target_properties(r-type_client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CLIENT_BUILD_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CLIENT_BUILD_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CLIENT_BUILD_DIR}"
)
