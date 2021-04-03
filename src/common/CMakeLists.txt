cmake_minimum_required ( VERSION 3.11 )
project ( fury2021common CXX )

set ( COMMON_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/background.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collision_circle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collision_square.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collision_triangle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collisionshape.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_crunch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_destructable.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_door.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_lift.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_moveable.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_point.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_trigger.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamicobject_base.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_item_coin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_item_key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_staticobject.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/data_tileset.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/debug_log.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_debugbox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_functions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_texturemanager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_xmlcache.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sys_vars.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/system_helpers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_basecontrol.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_button.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_checkbox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_ddl_element.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_dropdownlist.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_editbox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_il_element.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_imagebutton.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_imagelist.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_label.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_panel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_skin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_window.cpp

  PARENT_SCOPE
)

set ( COMMON_HDR
  ${CMAKE_CURRENT_SOURCE_DIR}/background.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collision_circle.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collision_square.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collision_triangle.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_collisionshape.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_crunch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_destructable.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_door.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_lift.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_moveable.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_point.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_switchable.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamic_trigger.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_dynamicobject_base.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_item_coin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_item_key.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_staticobject.h
  ${CMAKE_CURRENT_SOURCE_DIR}/data_tileset.h
  ${CMAKE_CURRENT_SOURCE_DIR}/debug_log.h
  ${CMAKE_CURRENT_SOURCE_DIR}/entity_gfx.h
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_debugbox.h
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_functions.h
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_texturemanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/helper_xmlcache.h
  ${CMAKE_CURRENT_SOURCE_DIR}/str_helpers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sys_vars.h
  ${CMAKE_CURRENT_SOURCE_DIR}/system_paths.h
  ${CMAKE_CURRENT_SOURCE_DIR}/types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_basecontrol.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_button.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_checkbox.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_ddl_element.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_dropdownlist.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_editbox.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_il_element.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_imagebutton.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_imagelist.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_label.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_main.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_panel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_skin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ui_window.h

  PARENT_SCOPE
)

set ( COMMON_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
  
  PARENT_SCOPE
)