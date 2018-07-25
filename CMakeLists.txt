cmake_minimum_required(VERSION 2.8)

project(youbotBTExample)
SET(CMAKE_CXX_FLAGS -pthread)

set (CMAKE_CXX_STANDARD 11)
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)

set(VREP_API_DIR ${PROJECT_SOURCE_DIR}/vrep_api/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(core)

add_subdirectory(bt_editor)


#include_directories (/home/miccol/V-REP_PRO_EDU_V3_2_3_rev4_Linux/programming
#/remoteApi /home/miccol/V-REP_PRO_EDU_V3_2_3_rev4_Linux/programming
#/include)



INCLUDE_DIRECTORIES(${VREP_API_DIR})
set(VREP_HEADER_FILES vrep_api/extApi.h vrep_api/extApiPlatform.h vrep_api/extApiInternal.h vrep_api/v_repConst.h)

set(VREP_SOURCE_FILES vrep_api/extApi.c vrep_api/extApiPlatform.c)

set(HEADER_FILES ${VREP_API_DIR}/vrep_utilities.h)

set(SOURCE_FILES ${VREP_API_DIR}/vrep_utilities.cpp)

add_library( vrep  ${VREP_HEADER_FILES} ${VREP_SOURCE_FILES})


#add_executable(${PROJECT_NAME} "main.cpp" ${HEADER_FILES} ${SOURCE_FILES}  ${VREP_HEADER_FILES} ${VREP_SOURCE_FILES})
#set(CMAKE_PLATFORM_USES_PATH_WHEN_NO_SONAME FALSE)
#target_link_libraries(${PROJECT_NAME} vrep)
