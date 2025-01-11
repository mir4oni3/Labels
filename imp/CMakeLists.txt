file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../imp/lib/*.cpp)

add_library(LabelsLibrary STATIC ${SOURCES})
add_executable(LabelsApp ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp)
target_link_libraries(LabelsApp LabelsLibrary)

target_include_directories(LabelsLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)