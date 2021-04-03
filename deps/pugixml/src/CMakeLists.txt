cmake_minimum_required ( VERSION 3.11 )
project ( pugixml CXX )

set ( PUGI_SRC
  pugixml.cpp
)

set ( PUGI_HDR
  pugixml.hpp
  pugiconfig.hpp
)

add_library ( pugixml STATIC ${PUGI_SRC} )