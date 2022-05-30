This example reads from ROS topic "/image_raw" and feeds the data to our simulated verilog modules. 

I use a gazebo world with a camera to publish simulated video recording to ROS topics. 




roslaunch urdf_tutorial display.launch model:=car.urdf


rosrun gazebo_ros gazebo --verbose car.world
