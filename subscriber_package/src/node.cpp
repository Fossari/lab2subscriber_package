#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h" //PUT THIS HERE /ajf

// collection of variable names: /ajf
// 
float vroom = 0;
float swerve = 0; 
std::vector<float> flashy;
// flashy is the variable for laser scanner in my case /ajf


/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
// %Tag(CALLBACK)%

void flashyCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
flashy = msg-> ranges;//whenever callback handles a new laser scan message it saves the ranges into flashy /ajf
ROS_INFO("I got a laser"); // flashy callback is called when a message
                           // from the laser occurs. then print to screen
                           // to see wahat happens /ajf
}

void twistCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
vroom = msg->linear.x;
swerve = msg->angular.z;
  ROS_INFO("I heard: [%f] %f", msg->linear.x, msg-> angular.z);

}
// %EndTag(CALLBACK)%

int main(int argc, char **argv)
{

  ROS_INFO("node start");

  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */

   ros::Subscriber twisty_sub = n.subscribe("des_vel", 1, twistCallback);
   ros::Subscriber flashy_sub = n.subscribe("laser_1", 1, flashyCallback); //laser subscribe /ajf
// used to say "robot0/laser_1"

   ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
// %Tag(SPIN)%
ros::Rate loop_rate(10);
while (ros::ok())
   {

     /**
      * This is a message object. You stuff it with data, and then publish it.
      */
     geometry_msgs::Twist msg;
     msg.linear.x = vroom;
     msg.angular.z = swerve;
 
if (vroom > 0 && flashy [270/2] < 2) msg.linear.x = 0;

     /**
      * The publish() function is how you send messages. The parameter
      * is the message object. The type of this object must agree with the type
      * given as a template parameter to the advertise<>() call, as was done
      * in the constructor above.
      */
     pub.publish(msg);
 
     ros::spinOnce();
 
     loop_rate.sleep();
  
   }
 
// %EndTag(SPIN)%

  return 0;
}
