project(CTRL_test)

add_cpp_test(
  CTRL UT_Controller
  SOURCES
    UT_Controller.cpp
    ${SRC_PATH}/Controller.cpp
  LIBRARIES
    DOMN
    COMM
  INCDIRS
    ${EXT_INC_PATH}
)
