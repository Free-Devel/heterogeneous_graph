include_directories (${heterogeneous_graph_SOURCE_DIR}/include) 

add_executable (exception src/exception.cpp) 

target_link_libraries(exception heterogeneous_graph) 
target_link_libraries(exception ${Boost_LIBRARIES})
