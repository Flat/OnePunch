cmake_minimum_required (VERSION 3.4.2)
project (OnePunch)
aux_source_directory(./src SRC_LIST)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
#set(CMAKE_BUILD_TYPE Debug)

# wxWidgets
set(wxWidgets_USE_STATIC ON)
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED core base adv net)
include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
