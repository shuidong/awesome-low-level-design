cmake_minimum_required(VERSION 3.16)

# Project configuration for Cocos Creator 3.8 Win32 RenderTexture Example
project(CocosRenderTextureExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform specific settings for Win32
if(WIN32)
    set(CMAKE_SYSTEM_VERSION 10.0) # Windows 10 SDK
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

# Cocos2d-x paths (adjust these paths to match your Cocos Creator 3.8 installation)
set(COCOS2DX_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../cocos2d-x")
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

# Include Cocos2d-x
include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# Source files
set(GAME_SOURCE
    RenderTextureExample.cpp
    RenderTextureExample.h
)

# Include directories
set(GAME_INC_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${COCOS2DX_ROOT_PATH}/cocos"
    "${COCOS2DX_ROOT_PATH}/cocos/platform"
    "${COCOS2DX_ROOT_PATH}/cocos/audio/include"
    "${COCOS2DX_ROOT_PATH}/external"
)

# Create executable
add_executable(${PROJECT_NAME} ${GAME_SOURCE})

# Link libraries
target_link_libraries(${PROJECT_NAME} cocos2d)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${GAME_INC_DIRS})

# Compiler specific settings
if(MSVC)
    # Use multi-processor compilation
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
    
    # Enable debug information in release builds
    target_compile_options(${PROJECT_NAME} PRIVATE /Zi)
    
    # Set warning level
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
    
    # Link with Windows-specific libraries
    target_link_libraries(${PROJECT_NAME} 
        opengl32.lib
        glu32.lib
        winmm.lib
        Version.lib
    )
endif()

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy resources to output directory (if any)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources" 
         DESTINATION "${CMAKE_BINARY_DIR}/bin")
endif()

# Configuration-specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE COCOS2D_DEBUG=1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Platform definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    CC_TARGET_PLATFORM=CC_PLATFORM_WIN32
    WIN32
    _WINDOWS
    _UNICODE
    UNICODE
)

# OpenGL definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    GL_GLEXT_PROTOTYPES
    CC_ENABLE_OPENGL_COMMANDS
)

# Build configuration
message(STATUS "Building Cocos Creator 3.8 RenderTexture Example for Win32")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Cocos2d-x root: ${COCOS2DX_ROOT_PATH}")

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)