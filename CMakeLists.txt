cmake_minimum_required(VERSION 3.20)
project(ContourExtractor)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH "C:\\Users\\skrze\\Documents\\SFML-2.5.1;D:\\Kody\\ExtLibs\\SFML\\SFML-2.5.1")

find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE -lsfml-window -lsfml-system -lsfml-graphics)
endif()