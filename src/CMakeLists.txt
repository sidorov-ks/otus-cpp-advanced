cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(editor VERSION ${PROJECT_VESRION})

option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")

  add_custom_target(doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the Doxygen documentation")
endif (DOXYGEN_FOUND)

option(WITH_GTEST "Whether to build Gtest" ON)

add_executable(editor main.cpp controller.cpp controller.h document.cpp document.h document_view.cpp document_view.h
  primitive/circle.h primitive/circle.cpp primitive/line.h primitive/line.cpp primitive/point.h primitive/point.cpp primitive/primitive.h)

set_target_properties(editor PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(editor
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

if (MSVC)
  target_compile_options(editor PRIVATE
    /W4
    )
else ()
  target_compile_options(editor PRIVATE
    -Wall -Wextra -pedantic -Werror -fPIC
    )
endif ()

install(TARGETS editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)