add_executable(bulk_server main.cpp session.cpp session.h server.cpp server.h)

set_target_properties(bulk_server PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  LINKER_LANGUAGE CXX
  )

find_package(Boost REQUIRED) #  COMPONENTS asio

target_include_directories(bulk_server PRIVATE "${PROJECT_SOURCE_DIR}/lib" "${Boost_INCLUDE_DIRS}")
target_link_libraries(bulk_server PRIVATE async ${Boost_LIBRARIES})

if (MSVC)
  target_compile_options(bulk_server PRIVATE
    /W4
    )
else ()
  target_compile_options(bulk_server PRIVATE
    -Wall -Wextra -pedantic -Werror
    )
endif ()

install(TARGETS bulk_server RUNTIME DESTINATION bin)