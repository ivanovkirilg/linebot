if(POLICY CMP0135)
  # Use extraction time of dependencies to decide whether to rebuild them.
  # Makes sure that *downgrading* will also trigger a rebuild.
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

find_package(Python COMPONENTS Interpreter REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/0bdccf4aa2f5c67af967193caf31d42d5c49bde2.zip
)
FetchContent_Declare(
  zpp_bits
  URL https://github.com/eyalz800/zpp_bits/archive/refs/tags/v4.4.25.zip
)

FetchContent_MakeAvailable(googletest zpp_bits)

include_directories(${zpp_bits_SOURCE_DIR})

find_package(Doxygen QUIET)
