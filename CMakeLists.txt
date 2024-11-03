cmake_minimum_required(VERSION 3.16)
project(prj012)

set(EXECUTABLE_NAME "Game")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##########################################################################################
# Project output directories for all builds (Debug, Release, etc.)
##########################################################################################

# Define output directories using CMAKE_BINARY_DIR
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/MinSizeRel/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/MinSizeRel/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/MinSizeRel/Bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/RelWithDebInfo/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/RelWithDebInfo/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/RelWithDebInfo/Bin)

##########################################################################################
# Add dependencies
##########################################################################################

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty)

# Add SDL2
set(SDL_TEST OFF CACHE BOOL "Disbale build of SDL2_test library")
set(SDL_SHARED ON CACHE BOOL "Build SDL2 as a shared library")
set(SDL_STATIC OFF CACHE BOOL "Disbale build of static version of the library")
set(SDL2_DISABLE_INSTALL ON CACHE BOOL "Disable SDL2 installation")
add_subdirectory(${THIRD_PARTY_DIR}/SDL2 ${CMAKE_BINARY_DIR}/ThirdParty/SDL2)

# Add fmt
add_subdirectory(${THIRD_PARTY_DIR}/fmt ${CMAKE_BINARY_DIR}/ThirdParty/fmt)

# Config DiligentEngine build
set(DILIGENT_BUILD_SAMPLES OFF)
set(DILIGENT_NO_OPENGL ON)
set(DILIGENT_NO_VULKAN ON)
set(DILIGENT_NO_METAL ON)
set(DILIGENT_NO_GLSLANG ON)

# Add DiligentCore
add_subdirectory(${THIRD_PARTY_DIR}/DiligentCore ${CMAKE_BINARY_DIR}/ThirdParty/DiligentCore)

# Add DiligentTools
add_subdirectory(${THIRD_PARTY_DIR}/DiligentTools ${CMAKE_BINARY_DIR}/ThirdParty/DiligentTools)

# Add DiligentFX
add_subdirectory(${THIRD_PARTY_DIR}/DiligentFX ${CMAKE_BINARY_DIR}/ThirdParty/DiligentFX)

# Add sdl-imgui
add_subdirectory(${THIRD_PARTY_DIR}/sdl-imgui ${CMAKE_BINARY_DIR}/ThirdParty/sdl-imgui)
# Make imgui available to sdl-imgui
target_include_directories(sdl-imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty/DiligentTools/ThirdParty/imgui)
# Make SDL2 available to sdl-imgui
target_include_directories(sdl-imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/ThirdParty/SDL2/include)

##########################################################################################
# Project executable setup
##########################################################################################

# Adding our source files
set(PROJECT_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Application.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Application.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/MathDefines.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/RendererDecl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Stats.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Stats.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Window.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Window.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Format.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Logger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Logger.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/PlatformContext.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/PrimitiveTypes.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Time.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Time.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Utility.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Utility.h

  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Config.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/ApplicationBase.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Clock.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Error.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Error.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Guid.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Guid.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/IApplicationCore.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/IApplicationWindow.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/IRenderCore.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Log.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Log.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/NativeWindow.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/RenderDeviceType.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/ResourceManager.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/ResourceManager.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Time.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/Game.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/Game.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/GameCore.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/GameCore.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/GameRenderCore.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/GameRenderCore.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/GameWindow.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/GameWindow.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Math/Matrix.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Math/Vector.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Geometry.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Geometry.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Material.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Mesh.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Mesh.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/RenderCore.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/RenderCore.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/RenderDevice.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/RenderTypes.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/TexturedCube.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/TexturedCube.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/VertexComponent.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Materials/BasicMaterial.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Materials/BasicMaterial.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/BitFlag.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/File.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/File.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/Format.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/NonCopyable.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/NonMovable.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/Platform.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/PrimitiveTypes.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/Singleton.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/StringBuilder.h
  # ${CMAKE_CURRENT_SOURCE_DIR}/Source/Utility/StringHash.h
)

# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE})

# Set target compile options
# target_compile_options(${PROJECT_NAME} PRIVATE -DUNICODE)

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
  OUTPUT_NAME ${EXECUTABLE_NAME}

  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
  LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}

  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}

  ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL}
  LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL}
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL}

  ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}
  LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}
)

##########################################################################################
# Linking out libraries
##########################################################################################

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
  # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

# Link fmt
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)

# Link DiligentCore
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  Diligent-GraphicsEngineD3D11-shared
  Diligent-GraphicsEngineD3D12-shared
)

# Link DiligentFX
target_link_libraries(${PROJECT_NAME} 
  PRIVATE 
  DiligentFX
)

# Link sdl-imgui
target_link_libraries(${PROJECT_NAME} PRIVATE sdl-imgui)

##########################################################################################
# Post-Build Commands
##########################################################################################

if (WIN32)

# Copy SDL2 DLLs to the output directory
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:SDL2::SDL2>
          $<TARGET_FILE_DIR:${PROJECT_NAME}>)

  # Copy assets to the output directory
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/Assets/
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets/
          VERBATIM)

  # Copy DiligentEngine DLLs to the output directory
  copy_required_dlls(${PROJECT_NAME})

  endif ()
