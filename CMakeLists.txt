cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip VERSION ${PROJECT_VESRION})

option(WITH_GTEST "Whether to build Gtest" ON)

configure_file(version.h.in version.h)

add_executable(ip_filter main.cpp)
add_library(ip ip.cpp)

set_target_properties(ip_filter ip PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  )

target_include_directories(ip
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

target_link_libraries(ip_filter PRIVATE
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
  include(GoogleTest)
  gtest_discover_tests(test_ip)
endif ()

if (MSVC)
  target_compile_options(ip_filter PRIVATE
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
  target_compile_options(ip_filter PRIVATE
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

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)