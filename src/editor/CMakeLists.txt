cmake_minimum_required ( VERSION 3.11 )
project ( fury2021editor CXX )

set ( FURYED_SRC
  data_level.cpp
  date_dynamic_start.cpp
  dialog_crunch.cpp
  dialog_door.cpp
  dialog_lift.cpp
  dialog_point.cpp
  dialog_trigger.cpp
  furyed.cpp
  grid.cpp
  onframefunc_ed.cpp
  onrenderfunc_ed.cpp
  preferences_dlg.cpp
  save_dlg.cpp
  system_interfaces.cpp
  system_ui.cpp
  system_ui_dynamicslist.cpp
  system_ui_rightpanel.cpp
  system_ui_staticsimagelist.cpp
  system_uistatemanager.cpp
  system_vars.cpp
  toolbar.cpp
  toolbox.cpp
)

set ( FURYED_HDR
  data_dynamic_start.h
  data_level.h
  dialog_crunch.h
  dialog_door.h
  dialog_lift.h
  dialog_point.h
  dialog_trigger.h
  grid.h
  preferences_dlg.h
  save_dlg.h
  system_interfaces.h
  system_main.h
  system_ui.h
  system_ui_rightpanel.h
  system_uistatemanager.h
  toolbar.h
  toolbox.h
)

add_executable ( furyed WIN32
  ${FURYED_SRC}
  ${COMMON_SRC}
)

target_include_directories ( furyed
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${COMMON_DIR}
    ${BOX2D_INCLUDE_PATH}
    ${PUGI_INCLUDE_PATH}
    ${HGE_INCLUDE_PATH}
    ${PUGIXML_INCLUDE_PATH}
)

target_link_libraries ( furyed
  PRIVATE
    hge
    hgehelp
    pugixml
)

add_dependencies ( furyed
  Box2D
  hge
  hgehelp
  pugixml
)

source_group ( Sources FILES ${FURYED_SRC} ${FURYED_HDR} ${COMMON_SRC} )