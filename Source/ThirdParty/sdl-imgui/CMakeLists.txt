cmake_minimum_required(VERSION 3.16)

project(sdl-imgui CXX)

set(SOURCE
  imgui_impl_sdl.cpp
  imgui_impl_sdl.h
)

add_library(sdl-imgui STATIC
  ${SOURCE}
  ${INTERFACE}
  ${IMGUIZMO_QUAT_SOURCE}
)
