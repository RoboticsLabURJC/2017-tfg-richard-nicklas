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


## The complete simulated environment

Example 5 is ready and working, it achieves a complete simulated environment consisting on the following parts: 

·Gazebo world with a simulated camera: We record a simulated world with a simulated camera and publish the data to a ROS topic

·C++ Plugin: Reads the video from a ROS Topic and feeds the data to the simulated FPGA ("verilated" C++ code). Also offers a UI to handle all the other interactions with the simulated FPGA.

·Simulated FPGA: C++ simulation of the FPGA ran  Verilog code that handles de image processing.


# Update Verilog

Our first approach to example5 wasn't using the latest versions of the verilog code. This update provides more accurate calculations, bug fixes and a new interface that provides aditional information like distance to the centroid of the filtered pixels. 

·input capture_newframe // pulse 1 clk, when a frame has been sent
·output new_centroid
·output [2:0] proximity  

