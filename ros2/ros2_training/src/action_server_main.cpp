/*******************************************************************************
 * @file    action_server_main.cpp
 * @brief   ROS2 Action Server - Fibonacci Sequence Generator
 * 
 * This module demonstrates a simple ROS2 action server that generates Fibonacci
 * sequences. An action allows a client to request a long-running task and receive
 * periodic feedback about progress before getting the final result.
 * 
 * Key difference from services:
 * - Service: Client sends request, server responds once (request-reply)
 * - Action: Client sends goal, server sends periodic feedback updates, then final result
 * 
 * Copyright 2026 Syntellect Robotics Pvt Ltd
 * Licensed under the Apache License, Version 2.0
 * See http://www.apache.org/licenses/LICENSE-2.0 for details.
 ******************************************************************************/

#include <inttypes.h>                              // For integer format specifiers
#include <memory>                                  // For smart pointers
#include <thread>                                  // For threading
#include "example_interfaces/action/fibonacci.hpp" // Fibonacci action definition
#include "rclcpp/rclcpp.hpp"                       // Core ROS2 functionality
#include "rclcpp_action/rclcpp_action.hpp"         // ROS2 action library

/**
 * @brief ROS2 Action Server for Fibonacci Sequence Generation
 * 
 * This server listens for action requests to generate Fibonacci sequences.
 * When a client requests a sequence of order N, the server:
 * 1. Validates the request
 * 2. Generates the Fibonacci sequence progressively
 * 3. Sends feedback updates as each number is calculated
 * 4. Returns the complete sequence as the final result
 */
class MinimalActionServer : public rclcpp::Node
{
public:
  // Type aliases for cleaner code
  using Fibonacci = example_interfaces::action::Fibonacci;
  using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

  /**
   * @brief Constructor - sets up the action server
   * 
   * Creates the action server and registers callback functions for:
   * - Goal handling (validation)
   * - Cancel handling (stopping in-progress computations)
   * - Accepted handling (starting execution)
   */
  explicit MinimalActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : Node("minimal_action_server", options)
  {
    using namespace std::placeholders;

    // Create the action server named "fibonacci"
    // Register three callback functions:
    // - handle_goal: Called when client sends a goal request
    // - handle_cancel: Called when client requests to cancel
    // - handle_accepted: Called when goal is accepted and ready to execute
    this->action_server_ = rclcpp_action::create_server<Fibonacci>(
      this->get_node_base_interface(),
      this->get_node_clock_interface(),
      this->get_node_logging_interface(),
      this->get_node_waitables_interface(),
      "fibonacci",
      std::bind(&MinimalActionServer::handle_goal, this, _1, _2),
      std::bind(&MinimalActionServer::handle_cancel, this, _1),
      std::bind(&MinimalActionServer::handle_accepted, this, _1));
    
    RCLCPP_INFO(this->get_logger(), "Fibonacci action server started!");
  }

private:
  // The action server object
  rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;

  /**
   * @brief Handle incoming goal requests
   * 
   * This function is called whenever a client sends a goal request.
   * It validates the goal and decides whether to accept or reject it.
   * 
   * @param uuid Unique identifier for this goal
   * @param goal The goal request containing the "order" parameter
   * @return ACCEPT_AND_EXECUTE to accept, REJECT to refuse the goal
   */
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID & uuid,
    std::shared_ptr<const Fibonacci::Goal> goal)
  {
    // Log the received request
    RCLCPP_INFO(
      this->get_logger(), 
      "Received goal request: generate Fibonacci sequence of order %d", 
      goal->order);
    
    // Reject goals that are unreasonably large (to prevent system overload)
    if (goal->order > 9000) {
      RCLCPP_WARN(this->get_logger(), "Rejecting goal - order too large!");
      return rclcpp_action::GoalResponse::REJECT;
    }
    
    // Accept valid goals and prepare to execute them
    RCLCPP_INFO(this->get_logger(), "Goal accepted!");
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  /**
   * @brief Handle cancellation requests from the client
   * 
   * This function is called when a client sends a request to cancel
   * an in-progress goal.
   * 
   * @param goal_handle Handle to the goal being canceled
   * @return ACCEPT to allow cancellation, REJECT to deny it
   */
  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandleFibonacci> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Received cancellation request");
    // Accept cancellation requests
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  /**
   * @brief Execute the Fibonacci sequence generation
   * 
   * This function computes the Fibonacci sequence step by step:
   * - Starts with [0, 1]
   * - Each new number = sum of previous two numbers
   * - Sends feedback to client after each calculation
   * - Returns the complete sequence as the result
   * 
   * @param goal_handle Handle to the current goal
   */
  void execute(const std::shared_ptr<GoalHandleFibonacci> goal_handle)
  {
    RCLCPP_INFO(this->get_logger(), "Starting Fibonacci sequence calculation...");
    
    // Create a rate object to control the pace (1 calculation per second)
    rclcpp::Rate loop_rate(1);
    
    // Get the goal parameters from the client
    const auto goal = goal_handle->get_goal();
    
    // Create a feedback message to send progress updates to the client
    auto feedback = std::make_shared<Fibonacci::Feedback>();
    auto & sequence = feedback->sequence;
    
    // Initialize the Fibonacci sequence with the first two numbers
    sequence.push_back(0);
    sequence.push_back(1);
    
    // Create a result message for the final answer
    auto result = std::make_shared<Fibonacci::Result>();

    // Generate the Fibonacci sequence up to the requested order
    for (int i = 1; (i < goal->order) && rclcpp::ok(); ++i) {
      // Check if the client has requested cancellation
      if (goal_handle->is_canceling()) {
        // If cancelled, return the sequence computed so far
        result->sequence = sequence;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "Goal was canceled by client");
        return;
      }
      
      // Calculate the next Fibonacci number as sum of the previous two
      sequence.push_back(sequence[i] + sequence[i - 1]);
      
      // Send progress feedback to the client showing the current sequence
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(
        this->get_logger(), 
        "Progress: sequence size = %ld", 
        sequence.size());

      // Wait 1 second before calculating the next number
      loop_rate.sleep();
    }

    // If the loop completed successfully, mark the goal as succeeded
    if (rclcpp::ok()) {
      result->sequence = sequence;
      goal_handle->succeed(result);
      RCLCPP_INFO(
        this->get_logger(), 
        "Goal succeeded! Generated Fibonacci sequence of size %ld", 
        sequence.size());
    }
  }

  /**
   * @brief Handle accepted goals by starting execution in a separate thread
   * 
   * This function is called after a goal has been accepted (handle_goal returned ACCEPT).
   * It spawns a new thread to execute the goal so the main executor doesn't block.
   * 
   * @param goal_handle Handle to the accepted goal
   */
  void handle_accepted(const std::shared_ptr<GoalHandleFibonacci> goal_handle)
  {
    using namespace std::placeholders;
    
    // Important: Execute in a separate thread to avoid blocking the executor
    // detach() allows the thread to run independently
    std::thread{std::bind(&MinimalActionServer::execute, this, _1), goal_handle}.detach();
  }
};  // End of MinimalActionServer class

/**
 * @brief Main entry point of the ROS2 action server application
 * 
 * Initializes the ROS2 system, creates the action server,
 * and runs the event processing loop.
 */
int main(int argc, char ** argv)
{
  // Initialize ROS2
  rclcpp::init(argc, argv);

  // Create the action server node
  auto action_server = std::make_shared<MinimalActionServer>();

  // Keep the server running and processing action requests
  rclcpp::spin(action_server);

  // Cleanup and shutdown ROS2
  rclcpp::shutdown();
  return 0;
}