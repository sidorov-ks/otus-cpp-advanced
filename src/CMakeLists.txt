cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

option(WITH_GTEST "Whether to build Gtest" ON)

configure_file(version.h.in version.h)

add_executable(matrix main.cpp)
add_library(libmatrix matrix.hpp cell.hpp matrix_iter.hpp)

set_target_properties(matrix libmatrix PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(libmatrix
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(matrix PRIVATE
  libmatrix
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
  add_executable(test_matrix test.cpp)
  target_link_libraries(test_matrix gtest gtest_main libmatrix)
  set_target_properties(test_matrix PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )
  include(GoogleTest)
  gtest_discover_tests(test_matrix)
endif ()

if (MSVC)
  target_compile_options(matrix PRIVATE
    /W4
    )
  target_compile_options(libmatrix PRIVATE
    /W4
    )
  if (WITH_GTEST)
    target_compile_options(test_matrix PRIVATE
      /W4
      )
  endif ()
else ()
  target_compile_options(matrix PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  target_compile_options(libmatrix PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  if (WITH_GTEST)
    target_compile_options(test_matrix PRIVATE
      -Wall -Wextra -pedantic -Werror
      )
  endif ()
endif ()

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)