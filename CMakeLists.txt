project(heterogeneous_graph)

cmake_minimum_required(VERSION 2.8)
option(USE_DATASYNAPSE "Compile Datasynapse sources." OFF)
option(LINK_DS_DEBUG_LIB "Link Datasynapse debug version of libraries." OFF)

################################################################################
### boost
################################################################################
macro(set_boost)
   if(NOT DEFINED ENV{BOOST_ROOT} AND NOT DEFINED BOOST_ROOT)
      get_filename_component(boost_root "${repdir}/../Boost/boost_${boost_dir}" ABSOLUTE)
      set(BOOST_ROOT
         ${boost_root}
         CACHE PATH "Root of the boost libraries"
      )
   elseif(DEFINED ENV{BOOST_ROOT})
      set(BOOST_ROOT
         $ENV{BOOST_ROOT}
         CACHE PATH "Root of the boost libraries"
      )
   endif()
   set(BOOST_LIBRARYDIR
      ${BOOST_ROOT}/stage/lib
      CACHE PATH "Root of the boost libraries"
   )

   set(Boost_FIND_REQUIRED TRUE)
   set(Boost_FIND_QUIETLY FALSE)
   set(Boost_USE_STATIC_LIBS ON)
   set(Boost_USE_MULTITHREADED ON)
   set(Boost_USE_STATIC_RUNTIME OFF)
   set(Boost_ADDITIONAL_VERSIONS "1.63.0")

   find_package(Boost "1.64.0" REQUIRED COMPONENTS thread system)
   include_directories(${Boost_INCLUDE_DIRS})
   link_directories(${Boost_LIBRARY_DIRS})
endmacro()
################################################################################

file(GLOB HEADERS include/*.hpp)
file(GLOB SOURCES src/*.cpp)
file(GLOB CMD_HEADERS include/commands/*.hpp)
file(GLOB CMD_SOURCES src/commands/*.cpp)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
   
source_group("Header Files" FILES                  
    ${HEADERS}                            
)
source_group("Source Files" FILES
    ${SOURCES}
)
   
find_package(Threads REQUIRED)
set_boost()

set(flag64 OFF)
if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
   set(flag64 ON)
endif()

#if(BOOST_FOUND)
   include_directories(${heterogeneous_graph_SOURCE_DIR}/include)
   add_library(heterogeneous_graph ${SOURCES} ${HEADERS})

   set(arch_dir "x86")
   if(flag64)
      set(arch_dir "x64")
   endif()
   add_custom_command(TARGET heterogeneous_graph POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:heterogeneous_graph> "${CMAKE_SOURCE_DIR}/lib/${arch_dir}/$<CONFIG>/$<TARGET_FILE_NAME:heterogeneous_graph>"
   )  

   TARGET_LINK_LIBRARIES(heterogeneous_graph ${CMAKE_THREAD_LIBS_INIT})
   if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      TARGET_LINK_LIBRARIES(heterogeneous_graph rt)
   endif()

   add_subdirectory(examples)
   add_subdirectory(test)
#endif()
