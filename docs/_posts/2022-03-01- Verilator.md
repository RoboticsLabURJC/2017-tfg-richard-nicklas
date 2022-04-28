---
title: "Verilator"
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


## Veritalor

Verilator is an open-source software that allows for the conversion of verilog HDL to C++ cycle-accurate behavioral models, this allows us to run and simulate code made for hardware entirely within virtual environments. 

Until now most of the developments I've done on FPGA-Robotics projects ran exclusively on hardware, this proved to be a slow development environment and extremely difficult to test and debug. Being able to code on C++ vastly improves development speed and allows the use of many tools that were unavailable before. 

The idea is to be able to use this sumulation environment to easily test new concepts or ideas, use custom virtual inputs and outputs for the HDL designs and compare outputs with the code ran on hardware.

## Examples

I've been working on the [poc_verilator_simulation](https://github.com/JdeRobot/FPGA-robotics/tree/master/utils/poc_verilator_simulation) examples1/2 by dlobato. These examples work with the following verilog rtl designs for a pixel processor and an image filtered color detector

├── example1
│   └── TopPixelProcessor.v
└── example2
    ├── calc_filter.v
    ├── design_top.v
    ├── frame_buff.v
    └── image_proc.v

This proof of concept showcases how to build hardware simulators where inputs and outputs are not straighforward, such us images.

## New Developments

└── example3


With openCV I've modified example2 to improve on the complexity of the inputs/outputs to the simulators, instead of loading still images I've conected live feed from a webcam as an input to the "red ball detector". 


Code can be found [here](https://github.com/RoboticsLabURJC/2017-tfg-richard-nicklas/tree/master/verilator/poc_verilator_simulation/sim/example3).
