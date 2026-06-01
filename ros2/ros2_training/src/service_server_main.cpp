/*******************************************************************************
 * @file    service_server_main.cpp
 * @brief   ROS2 Service Server Implementation
 * 
 * This module demonstrates a ROS2 service server that handles client requests.
 * The server listens for "add_two_ints" service calls and responds with the
 * sum of two provided integers.
 * 
 * Copyright 2026 Syntellect Robotics Pvt Ltd
 * Licensed under the Apache License, Version 2.0
 * See http://www.apache.org/licenses/LICENSE-2.0 for details.
 ******************************************************************************/

#include <inttypes.h>          // For PRId64 format specifier for 64-bit integers
#include <memory>                // For smart pointers (std::shared_ptr)
#include "example_interfaces/srv/add_two_ints.hpp"  // AddTwoInts service definition
#include "rclcpp/rclcpp.hpp"     // Core ROS2 functionality

// Type alias for the AddTwoInts service interface
using AddTwoInts = example_interfaces::srv::AddTwoInts;

// Global node pointer used throughout the file (needed for logger access in the service handler)
rclcpp::Node::SharedPtr g_node = nullptr;

/**
 * @brief Service callback handler for AddTwoInts service requests
 * 
 * This function is invoked whenever a client sends a request to the service.
 * It receives two integers from the client, computes their sum, and returns
 * the result in the response.
 * 
 * @param request_header Metadata about the service request (unused in this example)
 * @param request Pointer to the service request containing integers 'a' and 'b'
 * @param response Pointer to the service response where the sum is stored
 */
void handle_service(
  const std::shared_ptr<rmw_request_id_t> request_header,
  const std::shared_ptr<AddTwoInts::Request> request,
  const std::shared_ptr<AddTwoInts::Response> response)
{
  (void)request_header;  // Suppress unused parameter warning
  
  // Log the received request values for debugging and verification
  RCLCPP_INFO(
    g_node->get_logger(),
    "request: %" PRId64 " + %" PRId64, request->a, request->b);
  
  // Perform the addition and store the result in the response
  response->sum = request->a + request->b;
}

/**
 * @brief Main entry point of the ROS2 service server application
 * 
 * Initializes the ROS2 system, creates a node, establishes a service server,
 * and runs the event processing loop until shutdown is requested.
 */
int main(int argc, char ** argv)
{
  // Initialize the ROS2 system with command-line arguments
  rclcpp::init(argc, argv);
  
  // Create a ROS2 node named "minimal_service"
  g_node = rclcpp::Node::make_shared("minimal_service");
  
  // Create the service server that listens on "add_two_ints"
  // Incoming requests are handled by the handle_service callback function
  auto server = g_node->create_service<AddTwoInts>("add_two_ints", handle_service);
  
  // Keep the node running and processing service requests
  rclcpp::spin(g_node);
  
  // Shutdown the ROS2 system gracefully
  rclcpp::shutdown();
  
  // Clean up the global node pointer
  g_node = nullptr;
  
  // Return success
  return 0;
}