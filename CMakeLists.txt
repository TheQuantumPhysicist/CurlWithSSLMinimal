cmake_minimum_required(VERSION 2.8)

project(CurlSSLMinimal)


add_executable(${PROJECT_NAME} "main.cpp")

if(WIN32)
    target_link_libraries(${PROJECT_NAME} -lcurl -lnettle -lhogweed -lgmp)
else()
    target_link_libraries(${PROJECT_NAME} -lcurl)
endif(WIN32)
