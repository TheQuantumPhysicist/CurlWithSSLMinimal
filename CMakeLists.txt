cmake_minimum_required(VERSION 2.8)

project(CurlSSLMinimal)


add_executable(${PROJECT_NAME} "main.cpp")

if(WIN32)
    execute_process(
        COMMAND ${PKG_CONFIG_EXECUTABLE} libcurl --libs
        OUTPUT_VARIABLE CURL_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

message(${CURL_LIBS})
message(${PKG_CONFIG_EXECUTABLE})

target_link_libraries(${PROJECT_NAME} ${CURL_LIBS})
else()
    target_link_libraries(${PROJECT_NAME} -lcurl)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(WIN32)
