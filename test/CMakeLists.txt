set(CMAKE_MACOSX_BUNDLE FALSE)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_test(TestCache test.cpp TestCache.cpp)
target_link_libraries(TestCache PRIVATE ${LIB_NAME})

add_test(TestCrypto test.cpp TestCrypto.cpp)
target_link_libraries(TestCrypto PRIVATE ${LIB_NAME})

add_test(TestFileStore test.cpp TestFileStore.cpp)
target_link_libraries(TestFileStore PRIVATE ${LIB_NAME})

add_test(TestSectionRecord test.cpp TestSectionRecord.cpp)
target_link_libraries(TestSectionRecord PRIVATE ${LIB_NAME})
