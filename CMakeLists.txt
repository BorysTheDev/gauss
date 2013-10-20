cmake_minimum_required(VERSION 2.6.4)

include_directories (.)

set (PROJECT gauss_scheme)
project (${PROJECT})

set(GAUSS ./main.cpp)

if(CMAKE_COMPILER_IS_GNUCC)
add_definitions (-Wall -pedantic -std=gnu++11 -O2)
endif()

set (HEADERS 
  ./io.h
  ./lu.h
  ./matrix_generator.h
  ./gauss.h
 ./synchronization.h
 ./mtlu.h
)
set (SOURCES ./main.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES})

#target_link_libraries(${PROJECT} pthread)