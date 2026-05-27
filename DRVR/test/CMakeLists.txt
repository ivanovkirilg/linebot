project(DRVR_test)

add_cpp_test(
  DRVR FT_Driver
  SOURCES
    FT_Driver.cpp
    ${SRC_PATH}/Driver.cpp
  LIBRARIES
    Driver_interface
    DOMN
    SYNC
)
