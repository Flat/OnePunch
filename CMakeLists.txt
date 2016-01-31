cmake_minimum_required (VERSION 3.4.2)
project (OnePunch)

aux_source_directory(./src SRC_LIST)

# wxWidgets
set(wxWidgets_USE_STATIC ON)
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED core base adv)
include(${wxWidgets_USE_FILE})

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIS_RUNTIME OFF)
find_package(Boost REQUIRED system)
INCLUDE_DIRECTORIES($Boost_INCLUDE_DIRS})


add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES})