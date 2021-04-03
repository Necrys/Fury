cmake_minimum_required ( VERSION 3.11 )

FetchContent_Declare(
  box2d
  GIT_REPOSITORY "https://github.com/Necrys/Box2D_v2.1.2.git"
)

FetchContent_GetProperties( box2d )

if ( NOT box2d_POPULATED )
  FetchContent_Populate( box2d )
  message( STATUS "Box2D source dir: ${box2d_SOURCE_DIR}" )
  message( STATUS "Box2D binary dir: ${box2d_BINARY_DIR}" )
endif()

set ( BOX2D_INCLUDE_PATH ${box2d_SOURCE_DIR}/Box2D PARENT_SCOPE )

set ( BOX2D_BUILD_EXAMPLES OFF CACHE INTERNAL "" )
set ( BOX2D_BUILD_SHARED OFF CACHE INTERNAL "" )
set ( BOX2D_BUILD_STATIC ON CACHE INTERNAL "" )

add_subdirectory ( ${box2d_SOURCE_DIR}/Box2D ${box2d_BINARY_DIR} )