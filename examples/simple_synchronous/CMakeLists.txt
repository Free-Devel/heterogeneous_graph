include_directories (${heterogeneous_graph_SOURCE_DIR}/include) 

add_executable (simple_synchronous src/simple_synchronous.cpp) 

target_link_libraries(simple_synchronous heterogeneous_graph) 
target_link_libraries(simple_synchronous ${Boost_LIBRARIES})
