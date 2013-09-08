cmake_minimum_required(VERSION 2.6.4)

include_directories (.)

set (PROJECT gauss_scheme)
project (${PROJECT})

set(GAUSS ./main.cpp)

add_definitions (-Wall -pedantic -std=gnu++11 -O2)

set (HEADERS 
  ./io.h
  ./local_synchronization.h
  ./lu.h
  ./matrix_generator.h
 
 ./synchronization.h
 ./threads.h
)
set (SOURCES ./main.cpp)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES})

#target_link_libraries(${PROJECT} pthread)