add_library(gpxx INTERFACE)
target_include_directories(gpxx INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_sources(gpxx INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fmt/format.cc)

add_library(chaiscript INTERFACE)
target_compile_definitions(chaiscript INTERFACE -DCHAISCRIPT_NO_THREADS)
target_link_libraries(chaiscript INTERFACE ${CMAKE_DL_LIBS})

# demo1
add_executable(gpxx-demo1 demo1.cpp)
target_link_libraries(gpxx-demo1 PRIVATE gpxx)

# demo2
add_executable(gpxx-demo2 demo2.cpp)
target_link_libraries(gpxx-demo2 PRIVATE gpxx)

# demo3
add_executable(gpxx-demo3 demo3.cpp)
target_link_libraries(gpxx-demo3 PRIVATE gpxx)

# chaiscript
add_executable(gpxx-chaiscript chaiscript.cpp)
target_link_libraries(gpxx-chaiscript PRIVATE gpxx chaiscript)
