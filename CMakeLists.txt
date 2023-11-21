cmake_minimum_required(VERSION 3.5)
project(artslam_wrapper)

cmake_policy(SET CMP0074 NEW)  # Use <PackageName>_ROOT variables
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17 -fopenmp -DBOOST_LOG_DYN_LINK")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(ament_cmake REQUIRED)
find_package(geodesy REQUIRED)
find_package(nmea_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(pcl_ros REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_sensor_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(pcl_msgs REQUIRED)
find_package(std_srvs REQUIRED)

#find_package(GTSAM 4 REQUIRED)
#include_directories(${GTSAM_INCLUDE_DIR})

#find_package(Boost REQUIRED COMPONENTS program_options log log_setup)
#include_directories(${Boost_INCLUDE_DIRS})

find_package(PCL 1.12.1 REQUIRED)
# include_directories(${PCL_INCLUDE_DIRS})
# message("${PROJECT_NAME} - PCL version ${PCL_VERSION}")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#find_package(Eigen3 REQUIRED)
#include_directories(${EIGEN3_INCLUDE_DIR})

find_library(LOTS_CORE_IO_LIBS REQUIRED
        NAMES lots_core_io_lib
        HINTS "${PROJECT_SOURCE_DIR}/../../LOTS-CORE/lib"
)

find_library(LOTS_CORE_UTILS_LIBS REQUIRED
        NAMES lots_core_utils_lib
        HINTS "${PROJECT_SOURCE_DIR}/../../LOTS-CORE/lib"
)
include_directories(${PROJECT_SOURCE_DIR}/../../LOTS-CORE/include
        ${PROJECT_SOURCE_DIR}/../../LOTS-CORE/src)

find_library(LOTS_LIB_FOUND REQUIRED
        NAMES lotslam_lib
        HINTS "${PROJECT_SOURCE_DIR}/../../LOTS-SLAM/lib")
include_directories(${PROJECT_SOURCE_DIR}/../../LOTS-SLAM/include
        ${PROJECT_SOURCE_DIR}/../../LOTS-SLAM/src)

include_directories(${PCL_INCLUDE_DIRS})
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

add_executable(artslam_wrapper_node src/artslam_wrapper_node.cpp
    src/controller/controller.cpp
#    src/controller/bridge_visualizer.cpp
    src/kernel/skeleton.cpp
    src/kernel/frontend.cpp
    src/kernel/backend.cpp
    src/kernel/loop_detector.cpp
)

install(TARGETS
    artslam_wrapper_node
    DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
    launch
    config
    DESTINATION share/${PROJECT_NAME}/
)

target_link_libraries(artslam_wrapper_node
    # ${PCL_LIBRARIES}
#    gtsam
    ${LOTS_CORE_IO_LIBS}
    ${LOTS_CORE_UTILS_LIBS}
    ${LOTS_LIB_FOUND}
    ${PCL_LIBRARIES}
)

ament_target_dependencies(artslam_wrapper_node
    rclcpp
    std_msgs
    tf2
    tf2_ros
    tf2_geometry_msgs
    tf2_sensor_msgs
    tf2_eigen
    nav_msgs
    sensor_msgs
    visualization_msgs
    pcl_msgs
    pcl_conversions
    std_srvs
)

ament_export_libraries(${PROJECT_NAME})
ament_package()
