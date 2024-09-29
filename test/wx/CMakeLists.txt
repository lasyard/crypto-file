find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)

include(${wxWidgets_USE_FILE})

add_test(TestWxString ../test.cpp TestWxString.cpp)
target_link_libraries(TestWxString PRIVATE ${wxWidgets_LIBRARIES})
