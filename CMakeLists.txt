project(heterogeneous_graph)

cmake_minimum_required(VERSION 2.8)
option(USE_DATASYNAPSE "Compile Datasynapse sources." OFF)
option(LINK_DS_DEBUG_LIB "Link Datasynapse debug version of libraries." OFF)

set(BOOST_ROOT "/home/mirko/Development/boost_1_58_0" CACHE PATH "Path to root of BOOST library.")

file(GLOB HEADERS include/*.hpp)
file(GLOB SOURCES src/*.cpp)
file(GLOB CMD_HEADERS include/commands/*.hpp)
file(GLOB CMD_SOURCES src/commands/*.cpp)
file(GLOB DS_HEADERS datasynapse/include/*.hpp)
file(GLOB DS_SOURCES datasynapse/src/*.cpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")

set(Boost_USE_STATIC_LIBS        ON)     
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(Boost_ADDITIONAL_VERSIONS "1.58.0")
find_package(Boost 1.58.0 COMPONENTS thread date_time system timer chrono serialization program_options filesystem)
          
set(DATASYNAPSE_SDK_ROOT $ENV{DATASYNAPSE_SDK_ROOT})
if(USE_DATASYNAPSE AND (IS_DIRECTORY ${DATASYNAPSE_SDK_ROOT}))
   set(DS_CPPDRIVER_INCLUDE_DIRS "${DATASYNAPSE_SDK_ROOT}/cppdriver/include")

   if(${WIN32})
      set(DS_CPPDRIVER_INCLUDE_DIRS ${DATASYNAPSE_SDK_ROOT}/cppdriver/include)
    
      if(${LINK_DS_DEBUG_LIB})
          set(DS_CPPDRIVER_LIBRARIES 
             "${DATASYNAPSE_SDK_ROOT}/cppdriver/lib/vc9/DSUtilVC9D.lib"
             "${DATASYNAPSE_SDK_ROOT}/cppdriver/lib/vc9/DSDriverVC9D.lib"
          )
      else(${LINK_DS_DEBUG_LIB})
          set(DS_CPPDRIVER_LIBRARIES 
             "${DATASYNAPSE_SDK_ROOT}/cppdriver/lib/vc9/DSUtilVC9.lib"
             "${DATASYNAPSE_SDK_ROOT}/cppdriver/lib/vc9/DSDriverVC9.lib"
          )
      endif(${LINK_DS_DEBUG_LIB})
   endif(${WIN32})
   set(HEADERS ${HEADERS} ${CMD_HEADERS} ${DETAIL_HEADERS} ${DS_HEADERS})
   set(SOURCES ${SOURCES} ${CMD_SOURCES} ${DETAIL_SOURCES} ${DS_SOURCES})
else(USE_DATASYNAPSE AND (IS_DIRECTORY ${DATASYNAPSE_SDK_ROOT}))
   set(HEADERS ${HEADERS} ${CMD_HEADERS} ${DETAIL_HEADERS})
   set(SOURCES ${SOURCES} ${CMD_SOURCES} ${DETAIL_SOURCES})
endif(USE_DATASYNAPSE AND (IS_DIRECTORY ${DATASYNAPSE_SDK_ROOT}))
   
source_group("Header Files" FILES                  
    ${HEADERS}                            
)
source_group("Source Files" FILES
    ${SOURCES}
)
   
#if(MSVC)
#  set(CMAKE_CXX_FLAGS_RELEASE  "/Z7")
#endif(MSVC)

if(Boost_FOUND)
   find_package(Threads REQUIRED)

   set(flag64 OFF)
   if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
      set(flag64 ON)
   endif()

   include_directories(${Boost_INCLUDE_DIRS})
   include_directories(${DS_CPPDRIVER_INCLUDE_DIRS})

   include_directories(${heterogeneous_graph_SOURCE_DIR}/include)
   include_directories(${heterogeneous_graph_SOURCE_DIR}/datasynapse/include)
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
endif()

