cmake_minimum_required(VERSION 3.6)

set(lib_name gles)
set(target_name ext_${lib_name})

project(${lib_name})

include(../../cmake/CocosExternalConfig.cmake)

add_library(${target_name} SHARED IMPORTED GLOBAL)

set_target_properties(${target_name} PROPERTIES
                      INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include/OGLES
                      )

set_target_properties(${target_name} PROPERTIES
                      IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/glew32.dll"
                      IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/glew32.lib"
                      )
