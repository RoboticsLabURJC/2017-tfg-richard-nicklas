---
title: "ROS & GAZEBO F1"
excerpt: "Trying to run the FPGA-Robotics F1 simulation"

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

# URDF

https://wiki.ros.org/urdf/Tutorials/Building%20a%20Visual%20Robot%20Model%20with%20URDF%20from%20Scratch

cd /opt/ros/noetic/share/urdf_tutorial

roslaunch urdf_tutorial display.launch model:=urdf/01-myfirst.urdf

This does three things. It

    Loads the specified model into the parameter server
    Runs nodes to publish sensor_msgs/JointState and transforms (more on these later)
    Starts Rviz with a configuration file 


roslaunch urdf_tutorial display.launch model:='$(find urdf_tutorial)/urdf/09-camera.urdf'

## F1

Trying to launch

CustomRobots/f1$ roslaunch launch/simple_circuit_basic.launch 

doesn't work,I'll try to launch the dockerized examples from "RoboticsAcademy".

https://jderobot.github.io/RoboticsAcademy/exercises/AutonomousCars/follow_line/

docker pull jderobot/robotics-academy:latesti

sudo docker run --rm -it --device /dev/dri -p 8000:8000 -p 2303:2303 -p 1905:1905 -p 8765:8765 -p 6080:6080 -p 1108:1108 jderobot/robotics-academy:latest ./start.sh
o
sudo docker run --rm -it -p 8000:8000 -p 2303:2303 -p 1905:1905 -p 8765:8765 -p 6080:6080 -p 1108:1108 jderobot/robotics-academy:latest ./start.sh

Couldn't quite make that work as the simulation image was black and there wasn't any new topic I could find with "rostopic list". this could be for multiple reasons, possibly related to the fact that I'm trying to run this simulation inside a container inside a virtual machine...

```
Starting Gzclient
/bin/sh: 1: /RoboticsAcademy/exercises/follow_line/web-template/launch: Permission denied
172.17.0.1 - - [01/Apr/2022 14:46:06] 172.17.0.1: Plain non-SSL (ws://) WebSocket connection
172.17.0.1 - - [01/Apr/2022 14:46:06] 172.17.0.1: Path: '/websockify'
172.17.0.1 - - [01/Apr/2022 14:46:06] connecting to: localhost:5900
Gazebo multi-robot simulator, version 11.10.2
Copyright (C) 2012 Open Source Robotics Foundation.
Released under the Apache 2 License.
http://gazebosim.org

[VGL] ERROR: Could not open display /dev/dri/card0.
gzclient: /usr/include/boost/thread/pthread/recursive_mutex.hpp:108: void boost::recursive_mutex::lock(): Assertion `!posix::pthread_mutex_lock(&m)' failed.
Aborted (core dumped)

```

Was planing to chase this issue when I realized that the topics this simulation was publishing to exist on the container and not my virtual machine, I would need a way to connect to that network to be able to receive the information in my listener.cpp

I decided to go in a different direction as on the output of the container ran exercise there was an interesting line

INSTRUCTION:  /opt/ros/noetic/bin/roslaunch ./RoboticsAcademy/exercises/follow_line/web-template/launch/simple_line_follower_ros_headless_default.launch

This seems to be the line that actually runs the simulation, so I cloned the RoboticsAcademy repo..


ranicklas@ubuntu:~/cosas/Repos$ /opt/ros/noetic/bin/roslaunch ./RoboticsAcademy/exercises/static/exercises/follow_line/web-template/launch/simple_line_follower_ros_headless_default.launch

...
[Err] [Server.cc:472] Could not open file[prototype_f1_circuit_headless_default.world]
...

ranicklas@ubuntu:~/cosas/Repos$ vi ./RoboticsAcademy/exercises/static/exercises/follow_line/web-template/launch/simple_line_follower_ros_headless_default.launch 

ranicklas@ubuntu:~/cosas/Repos$ export GAZEBO_RESOURCE_PATH=~/cosas/Repos/RoboticsAcademy/exercises/static/exercises/follow_line/web-template/launch/
ranicklas@ubuntu:~/cosas/Repos$ /opt/ros/noetic/bin/roslaunch ./RoboticsAcademy/exercises/static/exercises/follow_line/web-template/launch/simple_line_follower_ros_headless_default.launch



