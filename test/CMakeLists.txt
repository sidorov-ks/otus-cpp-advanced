include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_executable(test_async test.cpp)
target_include_directories(test_async PRIVATE "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(test_async gtest gtest_main async)
set_target_properties(test_async PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  )
include(GoogleTest)
gtest_discover_tests(test_async)

if (MSVC)
  target_compile_options(test_async PRIVATE
    /W4
    )
else ()
  target_compile_options(test_async PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
endif ()