// Standard C++ library header
#include <memory>        // For smart pointers (std::make_shared)

// ROS2 C++ client library headers
#include "rclcpp/rclcpp.hpp"              // Core ROS2 functionality
#include "std_msgs/msg/string.hpp"        // Standard String message type

// This allows us to use _1 as a placeholder for callback function parameters
// (Used in std::bind to indicate where the message argument goes)
using std::placeholders::_1;

/* This example creates a subclass of Node that acts as a Subscriber.
 * Unlike the Publisher, this node RECEIVES messages from a ROS2 topic.
 * It listens to the "topic" topic and processes incoming messages via a callback. */

class MinimalSubscriber : public rclcpp::Node
{
  public:
    // Constructor - initializes the subscriber when the node is created
    MinimalSubscriber()
    : Node("minimal_subscriber")  // Initialize the Node with name "minimal_subscriber"
    {
      // Create a subscription to listen for String messages on the "topic" topic
      // The "10" is the queue size (maximum number of messages to queue)
      // std::bind() connects the topic_callback member function to this subscription
      // _1 is a placeholder for the incoming message data
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    // This function is called automatically whenever a new message arrives on the subscribed topic
    // The 'msg' parameter contains the received message data
    void topic_callback(const std_msgs::msg::String & msg) const
    {
      // Log (print) the received message to the console
      RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
    }
    
    // Subscription object that listens for incoming String messages from the ROS2 network
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

// Main entry point of the ROS2 application
int main(int argc, char * argv[])
{
  // Initialize the ROS2 system with command-line arguments
  rclcpp::init(argc, argv);
  
  // Create and run the MinimalSubscriber node
  // std::make_shared() creates the node and manages its memory automatically
  // rclcpp::spin() keeps the node running and waits for messages to arrive
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  
  // Shutdown the ROS2 system cleanly
  rclcpp::shutdown();
  
  // Return success
  return 0;
}