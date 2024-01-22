#!cmake .
#
# This works with SystemC versions 2.3.2 and later, which have support for
# cmake and specify SystemC_CXX_STANDARD.
#
# REQUIREMENTS:
#   Define SYSTEMC_HOME to point to install directory of SystemC (not source)
#   that has been built with CMake.
#
# See $ENV{SCC_APPS}/cmake/ABOUT_CMAKE.md for more information.

cmake_minimum_required( VERSION 3.20 )

project(      event_channel
  VERSION     1.0 # n.n[.n[.n]]
  DESCRIPTION "A simple channel for events"
  LANGUAGES   CXX
  )
#-------------------------------------------------------------------------------
# Find project directory containing defaults
#-------------------------------------------------------------------------------
set( defaults "sc-project_defaults" )
set( _dir "${CMAKE_CURRENT_SOURCE_DIR}" )
cmake_path( GET _dir ROOT_PATH _root )
while( NOT EXISTS "${_dir}/cmake/${defaults}.cmake" )
  cmake_path( GET _dir PARENT_PATH _dir )
  if( "${_dir}" STREQUAL "${_root}" )
    message( FATAL_ERROR "Unable to find project working tree directory!" )
  endif()
endwhile()
set( WORKTREE_DIR "${_dir}" CACHE PATH "Contains cmake/${defaults}.cmake" )
list( PREPEND CMAKE_MODULE_PATH "${WORKTREE_DIR}/cmake" )
include( "${defaults}" )


# Packages
#find_package( fmt REQUIRED )

set_target( event_channel )
add_executable( event_channel )
target_include_directories( event_channel PRIVATE . )
target_sources( event_channel PRIVATE
  event_if.hpp
  event.hpp
  event.cpp
  top.hpp
  top.cpp
  main.cpp
)
add_test( NAME "${Target}" COMMAND "${Target}" )
set_tests_properties("${Target}" PROPERTIES PASS_REGULAR_EXPRESSION "Simulation PASSED" )

# vim:syntax=cmake:nospell
