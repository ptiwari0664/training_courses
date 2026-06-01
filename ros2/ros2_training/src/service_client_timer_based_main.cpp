/*******************************************************************************
 * @file    service_client_timer_based_main.cpp
 * @brief   ROS2 Service Client with Timer-Based Requests
 * 
 * This module demonstrates a ROS2 service client that sends asynchronous requests
 * to an "add_two_ints" service at regular intervals (every 2 seconds).
 * The client generates random integer values and processes the responses.
 * 
 * Copyright 2026 Syntellect Robotics Pvt Ltd
 * Licensed under the Apache License, Version 2.0
 * See http://www.apache.org/licenses/LICENSE-2.0 for details.
 ******************************************************************************/

#include <chrono>                           // For time-related functionality
#include <cstdlib>                          // For random number generation
#include <memory>                           // For smart pointers
#include <ctime>                            // For seeding random number generator

#include "example_interfaces/srv/add_two_ints.hpp"  // AddTwoInts service definition
#include "rclcpp/rclcpp.hpp"                // Core ROS2 functionality

// Allow us to use 'ms' suffix for milliseconds (e.g., 2000ms instead of 2000000000ns)
using namespace std::chrono_literals;

// Type alias for the AddTwoInts service interface
using AddTwoInts = example_interfaces::srv::AddTwoInts;

/**
 * @brief ROS2 Service Client Node
 * 
 * This node acts as a client that periodically sends requests to the 
 * "add_two_ints" service with random integer values.
 */
class MinimalServiceClient : public rclcpp::Node
{
public:
  // Constructor - initializes the service client and timer
  MinimalServiceClient()
  : Node("service_client_timer")
  {
    // Create a client for the "add_two_ints" service
    // This client will send requests to the service server
    client_ = this->create_client<AddTwoInts>("add_two_ints");
    
    // Create a timer that calls timer_callback() every 2 seconds
    // This ensures the service is called at regular intervals
    timer_ = this->create_wall_timer(
      2000ms, std::bind(&MinimalServiceClient::timer_callback, this));
    
    // Seed the random number generator with the current time
    // This ensures different random numbers on each program run
    srand(static_cast<unsigned>(time(nullptr)));
    
    RCLCPP_INFO(this->get_logger(), "Service client started. Will send requests every 2 seconds.");
  }

private:
  /**
   * @brief Timer callback function
   * 
   * Called every 2 seconds to send a new service request with random values.
   * The function waits for the service to be available before sending the request.
   */
  void timer_callback()
  {
    // Check if the service server is available
    if (!client_->wait_for_service(1s)) {
      RCLCPP_WARN(
        this->get_logger(),
        "Service 'add_two_ints' not available yet. Waiting...");
      return;
    }
    
    // Create a service request
    auto request = std::make_shared<AddTwoInts::Request>();
    
    // Generate random integers between 1 and 100
    request->a = rand() % 100 + 1;
    request->b = rand() % 100 + 1;
    
    // Log the request for debugging
    RCLCPP_INFO(
      this->get_logger(),
      "Sending request: %ld + %ld", request->a, request->b);
    
    // Send the request asynchronously
    // The response will be handled by response_callback
    auto future = client_->async_send_request(
      request,
      std::bind(&MinimalServiceClient::response_callback, this, std::placeholders::_1));
  }
  
  /**
   * @brief Callback function for service response
   * 
   * This function is called when the service server responds to our request.
   * It receives and logs the computed sum.
   * 
   * @param future The future object containing the service response
   */
  void response_callback(rclcpp::Client<AddTwoInts>::SharedFuture future)
  {
    // Get the response from the future
    auto response = future.get();
    
    // Log the result
    RCLCPP_INFO(
      this->get_logger(),
      "Received response: sum = %ld", response->sum);
  }

  // Client object that communicates with the service server
  rclcpp::Client<AddTwoInts>::SharedPtr client_;
  
  // Timer object that triggers requests at regular intervals
  rclcpp::TimerBase::SharedPtr timer_;
};

/**
 * @brief Main entry point of the ROS2 service client application
 * 
 * Initializes the ROS2 system, creates a service client node,
 * and runs the event processing loop until shutdown is requested.
 */
int main(int argc, char * argv[])
{
  // Initialize the ROS2 system with command-line arguments
  rclcpp::init(argc, argv);
  
  // Create and run the service client node
  // std::make_shared() creates the node and manages its memory automatically
  // rclcpp::spin() keeps the node running and processing callbacks
  rclcpp::spin(std::make_shared<MinimalServiceClient>());
  
  // Shutdown the ROS2 system gracefully
  rclcpp::shutdown();
  
  // Return success
  return 0;
}