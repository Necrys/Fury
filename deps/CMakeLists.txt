include ( FetchContent )

include ( box2d.cmake )
include ( hge.cmake )

add_subdirectory ( pugixml/src )
set ( PUGIXML_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pugixml/src PARENT_SCOPE )