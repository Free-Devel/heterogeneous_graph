include_directories (${heterogeneous_graph_SOURCE_DIR}/include) 

add_executable (synchronous src/synchronous.cpp) 

target_link_libraries(synchronous heterogeneous_graph) 
target_link_libraries(synchronous ${Boost_LIBRARIES})
