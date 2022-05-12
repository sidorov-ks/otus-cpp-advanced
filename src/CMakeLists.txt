cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VESRION})

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

configure_file(version.h.in version.h)

add_executable(print_ip main.cpp)
add_library(ip ip.hpp custom_type_traits.hpp)

set_target_properties(print_ip ip PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(ip
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(print_ip PRIVATE
  ip
  )

if (WITH_GTEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  add_executable(test_ip test.cpp)
  target_link_libraries(test_ip gtest gtest_main ip)
  set_target_properties(test_ip PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )
  include(GoogleTest)
  gtest_discover_tests(test_ip)
endif ()

if (MSVC)
  target_compile_options(print_ip PRIVATE
    /W4
    )
  target_compile_options(ip PRIVATE
    /W4
    )
  if (WITH_GTEST)
    target_compile_options(test_ip PRIVATE
      /W4
      )
  endif ()
else ()
  target_compile_options(print_ip PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  target_compile_options(ip PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  if (WITH_GTEST)
    target_compile_options(test_ip PRIVATE
      -Wall -Wextra -pedantic -Werror
      )
  endif ()
endif ()

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)