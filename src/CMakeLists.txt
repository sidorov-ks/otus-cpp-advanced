add_executable(bulk main.cpp)

set_target_properties(bulk PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(bulk PRIVATE "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(bulk PRIVATE async)

if (MSVC)
  target_compile_options(bulk PRIVATE
    /W4
    )
else ()
  target_compile_options(bulk PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
endif ()

install(TARGETS bulk RUNTIME DESTINATION bin)