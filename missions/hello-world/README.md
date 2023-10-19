# mission: hello-world

A hello world mission to provide basic control of a simulated vessel in Gazebo within a MOOS community using MOOS-IvP behaviors. 

* Asset 1 is a simulated vessel in Gazebo.

## Mission
### Launch the simulated vessel in Gazebo
* Terminal 1 (source ROS overlay)
  * $ roslaunch vessel_bringup hello-world.launch
### Launch the MOOS Community
* Terminal 2
  * $ ./launch.sh
* Terminal 3
  * $ uPokeDB WPT_REQUEST=true
  * Enter IP address:  `enter`
  * Enter Port number: `9001` `enter`
* pMarineViewer
  * DEPLOY in pMarineViewer




