cmake_minimum_required ( VERSION 3.11 )

FetchContent_Declare(
  hge
  GIT_REPOSITORY "https://github.com/Necrys/HGE.git"
)

FetchContent_GetProperties( hge )

if ( NOT hge_POPULATED )
  FetchContent_Populate( hge )
  message( STATUS "HGE source dir: ${hge_SOURCE_DIR}" )
  message( STATUS "HGE binary dir: ${hge_BINARY_DIR}" )
endif()

set ( HGE_INCLUDE_PATH ${hge_SOURCE_DIR}/include PARENT_SCOPE )

#set ( DXSDK_PATH ${DX9SDK_PATH} CACHE INTERNAL "" )
#set ( BOX2D_BUILD_SHARED OFF CACHE INTERNAL "" )
#set ( BOX2D_BUILD_STATIC ON CACHE INTERNAL "" )

add_subdirectory ( ${hge_SOURCE_DIR} ${hge_BINARY_DIR} )