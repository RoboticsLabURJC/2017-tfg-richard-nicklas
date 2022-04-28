---
title: "ROS & Gazebo"
excerpt: "Overview and setup"

sidebar:
  nav: "docs"

toc: true
toc_label: "TOC installation"
toc_icon: "cog"


categories:
- your category
tags:
- Sim
- video
- Computer Vision


author: R. Nicklas
pinned: false
---


## ROS

ROS is a set of common libraries and tools that allows to create complex robotic systems. It uses URDF to define the models.
ROS Noetic (release date: 2020) is the last ROS1 version. This final ROS1 version main’s goal is to provide Python3 support.
ROS Noetic’s EOL (End of Life) is scheduled for 2025. 


http://wiki.ros.org/ROS/Installation

```
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

http://wiki.ros.org/ROS/Tutorials/InstallingandConfiguringROSEnvironment

Create the ROS workspace 

```
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/
catkin_make
```

http://wiki.ros.org/ROS/Tutorials/NavigatingTheFilesystem

·Packages: Packages are the software organization unit of ROS code. Each package can contain libraries, executables, scripts, or other artifacts.
·Manifests (package.xml): A manifest is a description of a package. It serves to define dependencies between packages and to capture meta information about the package like version, maintainer, license, etc..


http://wiki.ros.org/ROS/Tutorials/CreatingPackage


```
 cd ~/catkin_ws/src
 catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
```
Now you need to build the packages in the catkin workspace:

```
 cd ~/catkin_ws
 catkin_make
```



http://wiki.ros.org/ROS/Tutorials/BuildingPackages
http://wiki.ros.org/ROS/Tutorials/UnderstandingNodes

run roscore
```
... logging to /home/ranicklas/.ros/log/91b030c6-aaa6-11ec-810d-d71d3eda739d/roslaunch-ubuntu-13826.log
Checking log directory for disk usage. This may take a while.
Press Ctrl-C to interrupt
Done checking log file disk usage. Usage is <1GB.

started roslaunch server http://ubuntu:44385/
ros_comm version 1.15.14


SUMMARY
========

PARAMETERS
 * /rosdistro: noetic
 * /rosversion: 1.15.14

NODES

auto-starting new master
process[master]: started with pid [13837]
ROS_MASTER_URI=http://ubuntu:11311/

setting /run_id to 91b030c6-aaa6-11ec-810d-d71d3eda739d
process[rosout-1]: started with pid [13847]
started core service [/rosout]
```

After running roscore we can see the new service from a different terminal with

```
rosnode list

rosnode info /rosout
```

rosrun allows you to use the package name to directly run a node within a package (without having to know the package path).

Usage:
```
rosrun [package_name] [node_name]
```
e.g.

rosrun turtlesim turtlesim_node
rosrun turtlesim turtle_teleop_key



http://wiki.ros.org/ROS/Tutorials/UnderstandingTopics


The turtlesim_node and the turtle_teleop_key node are communicating with each other over a ROS Topic. turtle_teleop_key is publishing the key strokes on a topic, while turtlesim subscribes to the same topic to receive the key strokes.


rqt_graph creates a dynamic graph of what's going on in the system. rqt_graph is part of the rqt package. Unless you already have it installed, run:

    $ sudo apt-get install ros-noetic-rqt
    $ sudo apt-get install ros-noetic-rqt-common-plugins

```
 rosrun rqt_graph rqt_graph
```

The rostopic tool allows you to get information about ROS topics. 
rostopic echo shows the data published on a topic. 
rostopic type returns the message type of any topic being published. 
rostopic pub publishes data on to a topic currently advertised.
	rostopic pub [topic] [msg_type] [args]
	rostopic pub -1 /turtle1/cmd_vel geometry_msgs/Twist -- '[2.0, 0.0, 0.0]' '[0.0, 0.0, 1.8]' 


http://wiki.ros.org/ROS/Tutorials/UnderstandingServicesParams
http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29

After including the source code we edit the CMakeFile and run catkin_make

```
CMake Error at /opt/ros/noetic/share/genmsg/cmake/genmsg-extras.cmake:94 (message):
add_message_files() directory not found:

```

It seems I missed some part of the tutorial and have not created the ROS msg and srv

http://wiki.ros.org/ROS/Tutorials/CreatingMsgAndSrv

·msg: msg files are simple text files that describe the fields of a ROS message. They are used to generate source code for messages in different languages.

·srv: an srv file describes a service. It is composed of two parts: a request and a response. 


Create a .msg file in package/msg

We need to make sure that the msg files are turned into source code for C++, Python, and other languages:

Open package.xml, and make sure these two lines are in it and

```
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
```

Note that at build time, we need "message_generation", while at runtime, we only need "message_runtime".

Open CMakeLists.txt
Add the message_generation dependency to the find_package call which already exists in your CMakeLists.txt so that you can generate messages.





https://www.youtube.com/watch?v=KAASuA3_4eg
https://www.youtube.com/watch?v=CwdbsvcpOHM

## Gazebo 

Gazebo is an open-source 3D robotics simulator that can use multiple high-performance physics engines, such as ODE, Bullet, etc (the default is ODE). It provides realistic rendering of environments including high-quality lighting, shadows, and textures. It can model sensors that "see" the simulated environment, such as laser range finders, cameras (including wide-angle), Kinect style sensors, etc.

http://gazebosim.org/tutorials?tut=ros_overview

http://wiki.ros.org/simulator_gazebo/Tutorials/Write%20a%20Gazebo%20Plugin%20that%20Talks%20to%20ROS

http://gazebosim.org/tutorials/?tut=ros_control



https://www.youtube.com/watch?v=laWn7_cj434


$ gazebo /usr/share/gazebo-11/worlds/seesaw.world


.sdf files describe the world being simulated as well as the models which can be their own sdf files. 

Every time Gazebo wants to interact with something outside of itself it needs to use a plugin. For example if we want to send inputs to control a robot or receive information about some simulated sensor 


Sensors

Gazebo has the ability to simulate sensors and publish their outputs to ROS topics. Each sensor must be attached to a link, so we need to add a <sensor> tag inside the Gazebo reference tag we want to add it to.
we need to specify the sensor type, the plugin required to connect it to ROS, and any parameters the sensor or the plugin require.


http://gazebosim.org/tutorials/?tut=ros_urdf

http://gazebosim.org/tutorials?tut=ros_control

http://gazebosim.org/tutorials?tut=ros_gzplugins



## Examples

https://www.ros.org/reps/rep-0103.html#suffix-frames


