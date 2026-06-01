/*******************************************************************************
 * @file    action_client_main.cpp
 * @brief   ROS2 Action Client - Fibonacci Sequence Requester
 * 
 * This module demonstrates a ROS2 action client that requests Fibonacci sequences
 * from an action server. The client sends a goal, receives periodic feedback about
 * progress, and finally gets the complete result.
 * 
 * This complements action_server_main.cpp - they work together as a pair:
 * - Server: Computes the Fibonacci sequence progressively
 * - Client: Requests the sequence and displays the results
 * 
 * Copyright 2026 Syntellect Robotics Pvt Ltd
 * Licensed under the Apache License, Version 2.0
 * See http://www.apache.org/licenses/LICENSE-2.0 for details.
 ******************************************************************************/

#include <inttypes.h>                              // For integer format specifiers
#include <memory>                                  // For smart pointers
#include <string>                                  // For std::string
#include <iostream>                                // For console output
#include "example_interfaces/action/fibonacci.hpp" // Fibonacci action definition
#include "rclcpp/rclcpp.hpp"                       // Core ROS2 functionality
#include "rclcpp_action/rclcpp_action.hpp"         // ROS2 action library

/**
 * @brief ROS2 Action Client for Fibonacci Sequence Requests
 * 
 * This client connects to the Fibonacci action server and requests sequences.
 * It demonstrates how to:
 * 1. Send goal requests to an action server
 * 2. Handle acceptance/rejection of goals
 * 3. Receive and process feedback updates
 * 4. Handle final results
 */
class MinimalActionClient : public rclcpp::Node
{
public:
  // Type aliases for cleaner code
  using Fibonacci = example_interfaces::action::Fibonacci;
  using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>;

  /**
   * @brief Constructor - sets up the action client
   * 
   * Creates a client to communicate with the Fibonacci action server
   * and sets up a timer to trigger the goal request.
   */
  explicit MinimalActionClient(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions())
  : Node("minimal_action_client", node_options), goal_done_(false)
  {
    // Create a client that connects to the "fibonacci" action server
    this->client_ptr_ = rclcpp_action::create_client<Fibonacci>(
      this->get_node_base_interface(),
      this->get_node_graph_interface(),
      this->get_node_logging_interface(),
      this->get_node_waitables_interface(),
      "fibonacci");

    // Create a timer that sends the goal request after 500 milliseconds
    // This gives the server time to start up
    this->timer_ = this->create_wall_timer(
      std::chrono::milliseconds(500),
      std::bind(&MinimalActionClient::send_goal, this));
    
    RCLCPP_INFO(this->get_logger(), "Action client initialized. Waiting to send goal...");
  }

  /**
   * @brief Check if the goal execution is complete
   * 
   * Used in the main loop to know when to exit the program.
   * Returns true once the action result has been received.
   */
  bool is_goal_done() const
  {
    return this->goal_done_;
  }

  /**
   * @brief Send a goal request to the action server
   * 
   * This function:
   * 1. Waits for the action server to be available
   * 2. Creates a goal request (Fibonacci sequence of order 10)
   * 3. Sends the goal asynchronously
   * 4. Registers callbacks for goal response, feedback, and result
   */
  void send_goal()
  {
    using namespace std::placeholders;

    // Cancel the timer so this function only runs once
    this->timer_->cancel();

    // Reset the goal done flag
    this->goal_done_ = false;

    // Check if the client was initialized properly
    if (!this->client_ptr_) {
      RCLCPP_ERROR(this->get_logger(), "Action client not initialized");
    }

    // Wait for the action server to be ready (with a 10-second timeout)
    if (!this->client_ptr_->wait_for_action_server(std::chrono::seconds(10))) {
      RCLCPP_ERROR(
        this->get_logger(), 
        "Fibonacci action server not available after 10 seconds. Giving up.");
      this->goal_done_ = true;
      return;
    }

    // Create the goal message
    // We're asking for the first 10 Fibonacci numbers
    auto goal_msg = Fibonacci::Goal();
    goal_msg.order = 10;

    RCLCPP_INFO(
      this->get_logger(), 
      "Action server is ready. Sending request for Fibonacci sequence of order 10");

    // Set up callback functions for different action events
    auto send_goal_options = rclcpp_action::Client<Fibonacci>::SendGoalOptions();
    
    // Called when server accepts or rejects the goal
    send_goal_options.goal_response_callback =
      std::bind(&MinimalActionClient::goal_response_callback, this, _1);
    
    // Called whenever the server sends feedback
    send_goal_options.feedback_callback =
      std::bind(&MinimalActionClient::feedback_callback, this, _1, _2);
    
    // Called when the server sends the final result
    send_goal_options.result_callback =
      std::bind(&MinimalActionClient::result_callback, this, _1);
    
    // Send the goal request to the server
    auto goal_handle_future = this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
  }

private:
  // Client object for communicating with the action server
  rclcpp_action::Client<Fibonacci>::SharedPtr client_ptr_;
  
  // Timer object that triggers the goal sending after a delay
  rclcpp::TimerBase::SharedPtr timer_;
  
  // Flag to track if the goal has been completed
  bool goal_done_;

  /**
   * @brief Callback when the server responds to the goal request
   * 
   * This function is called shortly after send_goal() to inform us
   * whether the server accepted our goal or rejected it.
   * 
   * @param goal_handle Handle to the goal if accepted, or null if rejected
   */
  void goal_response_callback(GoalHandleFibonacci::SharedPtr goal_handle)
  {
    if (!goal_handle) {
      // The server rejected our goal request
      RCLCPP_ERROR(
        this->get_logger(), 
        "Goal was rejected by the server. Request may have violated constraints.");
    } else {
      // The server accepted our goal and is now processing it
      RCLCPP_INFO(
        this->get_logger(), 
        "Goal accepted by server! Now waiting for feedback and results...");
    }
  }

  /**
   * @brief Callback for receiving feedback during goal execution
   * 
   * This function is called multiple times as the server makes progress
   * on computing the Fibonacci sequence. Each callback contains the
   * latest Fibonacci number that was calculated.
   * 
   * @param goal_handle Handle to the goal being executed
   * @param feedback The feedback message containing the current sequence
   */
  void feedback_callback(
    GoalHandleFibonacci::SharedPtr,
    const std::shared_ptr<const Fibonacci::Feedback> feedback)
  {
    // Display the most recent Fibonacci number in the sequence
    RCLCPP_INFO(
      this->get_logger(),
      "Feedback received - Latest Fibonacci number: %d",
      feedback->sequence.back());
  }

  /**
   * @brief Callback when the action execution is complete
   * 
   * This function is called when the server finishes the goal and
   * returns the final result. It handles different outcome types:
   * - SUCCEEDED: Goal completed successfully
   * - ABORTED: Goal failed during execution
   * - CANCELED: Goal was canceled by client or server
   * 
   * @param result Contains the completion status and final result
   */
  void result_callback(const GoalHandleFibonacci::WrappedResult & result)
  {
    // Mark that the goal is done
    this->goal_done_ = true;
    
    // Check the status of the goal execution
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        // Goal completed successfully - fall through to display results
        break;
      case rclcpp_action::ResultCode::ABORTED:
        // Server encountered an error during execution
        RCLCPP_ERROR(this->get_logger(), "Goal was aborted by the server");
        return;
      case rclcpp_action::ResultCode::CANCELED:
        // Goal was canceled (by client or server)
        RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
        return;
      default:
        // Unknown result code
        RCLCPP_ERROR(this->get_logger(), "Unknown result code");
        return;
    }

    // Display the complete Fibonacci sequence
    RCLCPP_INFO(this->get_logger(), "Goal succeeded! Complete Fibonacci sequence:");
    for (auto number : result.result->sequence) {
      RCLCPP_INFO(this->get_logger(), "  %d", number);
    }
  }
};  // End of MinimalActionClient class

/**
 * @brief Main entry point of the ROS2 action client application
 * 
 * Initializes the ROS2 system, creates the action client,
 * and processes callbacks until the goal is complete.
 */
int main(int argc, char ** argv)
{
  // Initialize ROS2
  rclcpp::init(argc, argv);
  
  // Create the action client node
  auto action_client = std::make_shared<MinimalActionClient>();

  // Keep processing callbacks until the goal is done
  // rclcpp::spin_some() processes pending callbacks but doesn't block
  while (!action_client->is_goal_done()) {
    rclcpp::spin_some(action_client);
  }

  // Cleanup and shutdown ROS2
  rclcpp::shutdown();
  return 0;
}