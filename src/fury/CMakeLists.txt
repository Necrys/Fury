cmake_minimum_required ( VERSION 3.11 )
project ( fury2021game CXX )

set ( FURY_SRC
  data_effect.cpp
  data_fireball.cpp
  data_level.cpp
  data_player.cpp
  entity_phys.cpp
  fury.cpp
  input_key_map.cpp
  onframefunc.cpp
  onrenderfunc.cpp
  rope.cpp
  system_collider.cpp
  system_gamestate_manager.cpp
  system_interfaces.cpp
)

set ( FURY_HDR
  data_effect.h
  data_fireball.h
  data_level.h
  data_objectdata.h
  data_player.h
  entity_phys.h
  fury.h
  input_key_map.h
  rope.h
  system_collider.h
  system_gamestate_manager.h
  system_interfaces.h
  system_keymap.h
  system_main.h
  version.h
)

add_executable ( fury WIN32
  ${FURY_SRC}
  ${COMMON_SRC}
)

target_compile_definitions ( fury
  PRIVATE
    NOEDIT
)

target_compile_options ( fury
  PRIVATE
    "/MP"
)

target_include_directories ( fury
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${COMMON_DIR}
    ${BOX2D_INCLUDE_PATH}
    ${PUGI_INCLUDE_PATH}
    ${HGE_INCLUDE_PATH}
    ${PUGIXML_INCLUDE_PATH}
)

target_link_libraries ( fury
  PRIVATE
    Box2D
    hge
    hgehelp
    pugixml
)

add_dependencies ( fury
  Box2D
  hge
  hgehelp
  pugixml
)

source_group ( fury FILES ${FURY_SRC} ${FURY_HDR} ${COMMON_SRC} )