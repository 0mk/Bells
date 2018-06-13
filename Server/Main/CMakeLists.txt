cmake_minimum_required(VERSION 2.8)

project(Bells)

FIND_PACKAGE(BISON REQUIRED)
SET(BisonOutput ${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.tab.cc)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${BisonOutput}
      COMMAND ${BISON_EXECUTABLE}
              --defines=${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.tab.hh
              --output=${BisonOutput}
              ${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.yy
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.yy
      COMMENT "Generating Command Parser"
    )
ENDIF()

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.l.cc)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
      OUTPUT ${FlexOutput}
      COMMAND ${FLEX_EXECUTABLE}
              --c++
              --outfile=${FlexOutput}
              --header-file=${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.l.h
              ${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.l
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ReadPn.l
      COMMENT "Generating Command Scanner"
    )
ENDIF()

set(Bells_Sources
Bells.C
Composition.C
CompositionData.C
CompositionParser.C
Method.C
Ringer.C
Log.C
tinythread.C
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable (Bells ${Bells_Sources} ${BisonOutput} ${FlexOutput})
target_link_libraries (Bells LINK_PUBLIC Midi Socket pthread asound)


########### install files ###############

install(TARGETS Bells DESTINATION /usr/local/bin)
