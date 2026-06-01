cmake_minimum_required(VERSION 3.8)
project(ros2_training)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)

add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

add_executable(listener src/subscriber_member_function.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

add_executable(service_server src/service_server_main.cpp)
ament_target_dependencies(service_server rclcpp std_msgs example_interfaces)

add_executable(service_client src/service_client_timer_based_main.cpp)
ament_target_dependencies(service_client rclcpp example_interfaces)

add_executable(action_server src/action_server_main.cpp)
ament_target_dependencies(action_server rclcpp rclcpp_action example_interfaces)

add_executable(action_client src/action_client_main.cpp)
ament_target_dependencies(action_client rclcpp rclcpp_action example_interfaces)

install(TARGETS
  talker
  listener
  service_server
  service_client
  action_server
  action_client
  DESTINATION lib/${PROJECT_NAME})

ament_package()
