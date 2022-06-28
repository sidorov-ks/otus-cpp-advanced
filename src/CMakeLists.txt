cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VESRION})

option(WITH_GTEST "Whether to build Gtest" ON)

configure_file(version.h.in version.h)

add_executable(bulk main.cpp io.h io.cpp)
add_library(libbulk block.h block_processor.h block_processor.cpp)

set_target_properties(bulk libbulk PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(libbulk
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(bulk PRIVATE
  libbulk
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
  add_executable(test_bulk test.cpp)
  target_link_libraries(test_bulk gtest gtest_main libbulk)
  set_target_properties(test_bulk PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )
  include(GoogleTest)
  gtest_discover_tests(test_bulk)
endif ()

if (MSVC)
  target_compile_options(bulk PRIVATE
    /W4
    )
  target_compile_options(libbulk PRIVATE
    /W4
    )
  if (WITH_GTEST)
    target_compile_options(test_bulk PRIVATE
      /W4
      )
  endif ()
else ()
  target_compile_options(bulk PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  target_compile_options(libbulk PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  if (WITH_GTEST)
    target_compile_options(test_bulk PRIVATE
      -Wall -Wextra -pedantic -Werror
      )
  endif ()
endif ()

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)