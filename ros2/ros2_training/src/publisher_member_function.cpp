// Standard C++ library headers
#include <chrono>        // For time-related functionality
#include <functional>    // For std::bind() to bind member functions
#include <memory>        // For std::make_shared and smart pointers
#include <string>        // For std::string class

// ROS2 C++ client library headers
#include "rclcpp/rclcpp.hpp"              // Core ROS2 functionality
#include "std_msgs/msg/string.hpp"        // Standard String message type

// Allow us to use 'ms' suffix for milliseconds (e.g., 500ms instead of 500000000ns)
using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer.
 * 
 * A "Node" is the basic building block in ROS2. This node acts as a Publisher
 * that sends messages to a ROS2 topic at regular intervals. */

// Create a custom Node class called MinimalPublisher
class MinimalPublisher : public rclcpp::Node
{
  public:
    // Constructor - initializes the publisher and timer when the node is created
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)  // Initialize the Node with name "minimal_publisher"
    {
      // Create a publisher that sends String messages on the "topic" topic
      // The "10" is the queue size (maximum number of messages to queue if subscribers are slow)
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
      
      // Create a timer that calls timer_callback() every 500 milliseconds
      // std::bind() connects the member function to the callback
      timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    // This function is called every 500ms by the timer
    void timer_callback()
    {
      // Create a new String message object
      auto message = std_msgs::msg::String();
      
      // Fill the message with a greeting and an incrementing counter
      message.data = "Hello, world! " + std::to_string(count_++);
      
      // Log the published message to the console for debugging
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      
      // Publish (send) the message to all subscribers listening on "topic"
      publisher_->publish(message);
    }
    
    // Timer object that triggers the callback at regular intervals
    rclcpp::TimerBase::SharedPtr timer_;
    
    // Publisher object that sends String messages to the ROS2 network
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    
    // Counter that increments with each published message
    size_t count_;
};

// Main entry point of the ROS2 application
int main(int argc, char * argv[])
{
  // Initialize the ROS2 system with command-line arguments
  rclcpp::init(argc, argv);
  
  // Create and run the MinimalPublisher node
  // std::make_shared() creates the node and manages its memory automatically
  // rclcpp::spin() keeps the node running and processes callbacks
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  
  // Shutdown the ROS2 system cleanly
  rclcpp::shutdown();
  
  // Return success
  return 0;
}