# Install script for directory: C:/Users/ccair/Documents/Programs/GitHub/PhysicsPinball/fisica2-CleanTemplate/vcpkg_installed/x86-windows/vcpkg/blds/raylib/src/5.0-24e38c87d8.clean/src/external/glfw

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/ccair/Documents/Programs/GitHub/PhysicsPinball/fisica2-CleanTemplate/vcpkg_installed/x86-windows/vcpkg/pkgs/raylib_x86-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/ccair/Documents/Programs/GitHub/PhysicsPinball/fisica2-CleanTemplate/vcpkg_installed/x86-windows/vcpkg/blds/raylib/x86-windows-rel/raylib/external/glfw/src/cmake_install.cmake")
endif()

