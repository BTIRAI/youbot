cmake_minimum_required(VERSION 2.8)

project(youbotVrepTests)

add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)

set(VREP_HEADER_FILES extApi.h extApiPlatform.h extApiInternal.h v_repConst.h)

set(VREP_SOURCE_FILES extApi.c extApiPlatform.c)

set(HEADER_FILES vrep_utilities.h)

set(SOURCE_FILES vrep_utilities.cpp)

include_directories (/Users/miccol/Downloads/V-REP_PRO_EDU_V3_2_3_rev4_Mac/programming
/remoteApi /Users/miccol/Downloads/V-REP_PRO_EDU_V3_2_3_rev4_Mac/programming
/include)

add_library( vrep ${VREP_SOURCE_FILES} ${VREP_HEADER_FILES} ${VREP_SOURCE_FILES})


add_executable(${PROJECT_NAME} "main.cpp" ${HEADER_FILES} ${SOURCE_FILES})
set(CMAKE_PLATFORM_USES_PATH_WHEN_NO_SONAME FALSE)
target_link_libraries(${PROJECT_NAME} vrep)
