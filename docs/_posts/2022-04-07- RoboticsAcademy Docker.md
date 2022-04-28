---
title: "Gazebo/ROS Camera"
excerpt: "New developments"

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


## Docker

After the issues found with my local environment I'll be trying to work inside the Robotics Academy Docker container

```
ranicklas@ubuntu:~$ sudo docker ps
CONTAINER ID   IMAGE                              COMMAND                  CREATED          STATUS          PORTS                                                                                                                                                                                                                                                                                            NAMES
7827ecfa03b0   jderobot/robotics-academy:latest   "./entrypoint.sh ./s…"   11 seconds ago   Up 10 seconds   1831/tcp, 0.0.0.0:1108->1108/tcp, :::1108->1108/tcp, 0.0.0.0:1905->1905/tcp, :::1905->1905/tcp, 1904/tcp, 0.0.0.0:2303->2303/tcp, :::2303->2303/tcp, 0.0.0.0:6080->6080/tcp, :::6080->6080/tcp, 0.0.0.0:8000->8000/tcp, :::8000->8000/tcp, 0.0.0.0:8765->8765/tcp, :::8765->8765/tcp, 2304/tcp   exciting_hugle

ranicklas@ubuntu:~$ sudo docker attach exciting_hugle



Traceback (most recent call last):
  File "manager.py", line 772, in <module>
    server.run_server()
  File "manager.py", line 767, in run_server
    asyncio.get_event_loop().run_forever()

```

No Response... 
Maybe ssh?

```
sudo docker inspect -f "{{ .NetworkSettings.IPAddress }}" exciting_hugle
172.17.0.2

ssh 172.17.0.2
ssh: connect to host 172.17.0.2 port 22: Connection refused

```

```
ranicklas@ubuntu:~$ sudo docker exec -it eager_shaw service ssh status
ssh: unrecognized service
ranicklas@ubuntu:~$ sudo docker exec -it eager_shaw bash
root@58323727017c:/# 
```

Inside the container we see the gzserver command

gzserver --verbose -e ode prototype_f1_circuit_headless_default.world -s /opt/ros/noetic/lib/libgazebo_ros_paths_plugin.so -s /opt/ros/noetic/lib/libgazebo_ros_api_plugin.so __name:=gazebo __log:=/root/.ros/log/780ad446-b1e0-11ec-9745-0242ac110002/gazebo-2.log

find the cam.



## CAM From scratch

Too many issues from a very complex working environment left me wasting too much time without tangible advances, I've decided to try again to create the camera from scratch. This way I'll be able to go forward and understand step by step 

Found this turorial: 
https://gazebosim.org/tutorials?tut=camera_save&cat=sensors

created the "camera" package in the catkin_ws

gazebo --verbose camera.world

This tutorial proved useful and I was able to make it work the recorded video is stored as images in /tmp, I'm trying to link a pluging so the camera publishes the information to a ros topic. 

...
[FATAL] [1649078766.561549152]: A ROS node for Gazebo has not been initialized, unable to load plugin. Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)
...

I'll check this one next http://gazebosim.org/tutorials/?tut=ros_plugins

Relevant part to fix the issue: 

ROS Node Note

All gazebo-ros plugins should check if the ROS node has already been initialized in their Load() function. The initialization of the ROS node is performed automatically when you run

```
rosrun gazebo_ros gazebo
```
or use the generic empty.world launch file. The gazebo_ros/src/gazebo_ros_api_plugin.cpp should be the only place in Gazebo that calls ros::init().


In our case this meant runing the following commant. 

```
rosrun gazebo_ros gazebo --verbose camera.world
```

It works!!

ranicklas@ubuntu:~/catkin_ws/src/camera/worlds$ rostopic list
/camera_info
/clock
/gazebo/link_states
/gazebo/model_states
/gazebo/parameter_descriptions
/gazebo/parameter_updates
/gazebo/performance_metrics
/gazebo/set_link_state
/gazebo/set_model_state
/image_raw
/image_raw/compressed
/image_raw/compressed/parameter_descriptions
/image_raw/compressed/parameter_updates
/image_raw/compressedDepth
/image_raw/compressedDepth/parameter_descriptions
/image_raw/compressedDepth/parameter_updates
/image_raw/theora
/image_raw/theora/parameter_descriptions
/image_raw/theora/parameter_updates
/rosout
/rosout_agg


More info to link this to a robot URDF

http://gazebosim.org/tutorials?tut=ros_gzplugins#Camera

Now we continue with the development of the listener that will subscribe to the camera_info and image_raw topics!


Created a new project for the cam_listener.cpp from the following example

http://wiki.ros.org/image_transport/Tutorials/SubscribingToImages
http://ros.org/wiki/image_transport 


Important to edit the CMakeLists.txt with OpenCV dependencies, image_transport and cv_bridge

```
find_package(catkin REQUIRED COMPONENTS
    ...
    image_transport
    ...
)

find_package(OpenCV REQUIRED)
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )


add_executable(cam_listener src/camera_listener.cpp)
target_link_libraries(cam_listener ${catkin_LIBRARIES}  ${OpenCV_LIBS})
```

and the package.xml file with

```
<build_depend>image_transport</build_depend>
<build_depend>cv_bridge</build_depend>

<exec_depend>cv_bridge</exec_depend>
<exec_depend>image_transport</exec_depend>
```

Managed to compile with these changes, but got a warining on execution

ranicklas@ubuntu:~/catkin_ws$ ./devel/lib/beginner_tutorials/cam_listener
[ WARN] [1649410213.029899892, 6745.888000000]: [image_transport] It looks like you are trying to subscribe directly to a transport-specific image topic 'image/raw', in which case you will likely get a connection error. Try subscribing to the base topic 'image' instead with parameter ~image_transport set to 'raw' (on the command line, _image_transport:=raw). See http://ros.org/wiki/image_transport for details.


fixed the line to subscribe to "image_raw" and it works! im able to see on a new windowed image the objects I simulate in Gazebo!

# One project to rule them all

Now I need to include all these developments into the example4 simulated verilog code so I can feed these images. 
First, we need to include ROS. In the Makefile we add the ros include folder to the CXXFLAGS environment variable and the needed libraries to the LIBS environment variable

```
ROS_INCLUDE_DIR = /opt/ros/noetic/include/
ROS_LIB_DIR = /opt/ros/noetic/lib


CXXFLAGS = -I$(ROS_INCLUDE_DIR) -I$(INC_DIR) -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -DASSETS_DIR=\\\"$(ASSETS_DIR)\\\" -g
LIBS = -L$(ROS_LIB_DIR) -lroscpp -lrosconsole -limage_transport -lcv_bridge


```


