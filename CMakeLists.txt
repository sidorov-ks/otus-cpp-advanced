cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(allocator VERSION ${PROJECT_VESRION})

option(WITH_GTEST "Whether to build Gtest" ON)

configure_file(version.h.in version.h)

add_executable(alloc_bin main.cpp)
add_library(alloc linear_allocator.hpp)

set_target_properties(alloc_bin alloc PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(alloc
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(alloc_bin PRIVATE
  alloc
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
  add_executable(test_alloc test.cpp)
  target_link_libraries(test_alloc gtest gtest_main alloc)
  include(GoogleTest)
  gtest_discover_tests(test_alloc)
endif ()

if (MSVC)
  target_compile_options(alloc_bin PRIVATE
    /W4
    )
  target_compile_options(alloc PRIVATE
    /W4
    )
  if (WITH_GTEST)
    target_compile_options(test_alloc PRIVATE
      /W4
      )
  endif ()
else ()
  target_compile_options(alloc_bin PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  target_compile_options(alloc PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  if (WITH_GTEST)
    target_compile_options(test_alloc PRIVATE
      -Wall -Wextra -pedantic -Werror
      )
  endif ()
endif ()

install(TARGETS alloc_bin RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)