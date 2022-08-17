add_executable(fashio_mnist main.cpp sample.h model.h model.cpp)

set_target_properties(fashio_mnist PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(fashio_mnist Eigen3::Eigen)

if (MSVC)
  target_compile_options(fashio_mnist PRIVATE
    /W4
    )
else ()
  target_compile_options(fashio_mnist PRIVATE
    -Wall -Wextra -pedantic -Werror
    -fno-math-errno -ffast-math
    )
endif ()

install(TARGETS fashio_mnist RUNTIME DESTINATION bin)