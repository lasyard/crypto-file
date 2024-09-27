add_lib(${LIB_NAME} Cache.cpp Crypto.cpp FileStore.cpp SectionRecord.cpp)

target_link_libraries(${LIB_NAME} PUBLIC libcryptopp.a)
