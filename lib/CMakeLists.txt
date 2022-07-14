add_library(async async.h async.cpp block.h block_processor.h block_processor.cpp
  io/producers.h io/producers.cpp io/consumers.h io/consumers.cpp)

set_target_properties(async PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

target_include_directories(async
  PRIVATE "${CMAKE_BINARY_DIR}"
  )

if (MSVC)
  target_compile_options(async PRIVATE
    /W4
    )
else ()
  target_compile_options(async PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
endif ()