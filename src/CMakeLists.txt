cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VESRION})

option(WITH_GTEST "Whether to build Gtest" ON)

configure_file(version.h.in version.h)

add_executable(bayan main.cpp)
add_library(libbayan search_config.h bayan.h bayan.cpp hashing.h hashing.cpp)

set_target_properties(bayan libbayan PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  )

find_package(Boost COMPONENTS program_options filesystem log REQUIRED)
find_package(OpenSSL REQUIRED)

target_include_directories(libbayan
  PRIVATE "${CMAKE_BINARY_DIR}" "${Boost_INCLUDE_DIRS}"
  )

target_link_libraries(bayan PRIVATE
  libbayan ${Boost_LIBRARIES}
  )

target_link_libraries(libbayan
  ${Boost_LIBRARIES} OpenSSL::SSL
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
  add_executable(test_bayan test.cpp)
  target_link_libraries(test_bayan gtest gtest_main gmock libbayan ${Boost_LIBRARIES})
  set_target_properties(test_bayan PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
  include(GoogleTest)
  gtest_discover_tests(test_bayan)
endif ()

if (MSVC)
  target_compile_options(bayan PRIVATE
    /W4
    )
  target_compile_options(libbayan PRIVATE
    /W4
    )
  if (WITH_GTEST)
    target_compile_options(test_bayan PRIVATE
      /W4
      )
  endif ()
else ()
  target_compile_options(bayan PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  target_compile_options(libbayan PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
  if (WITH_GTEST)
    target_compile_options(test_bayan PRIVATE
      -Wall -Wextra -pedantic -Werror
      )
  endif ()
endif ()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)