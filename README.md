# MOOS-IvP-Sim-Gazebo

This repo contains the basic autonomy interface for controlling a simulated vessel in Gazebo. This repo is designed to be used with moos-ivp-gateway <link>. Be sure to source each moos-ivp tree properly in .bashrc. This repo is also intended to be used with the ROS package gazebo_sim_moos_ivp <link>. 

# Source 
  * iGazeboVessel
    * An interface to control a simulated Gazebo vessel from the MOOS-IvP pMarinePID `DESIRED_THRUST` and `DESIRED_RUDDER` output data

  * pParseGateway
    * A basic parsing application for ingesting simulated vessel sensor data from Gazebo
    * Copy into your project (moos-ivp-`your-project`/src) and edit as needed

# Missions
  * hello-world
    * Example mission to explore a single vessel in Gazebo controlled by a single MOOS communitys
  * multi-robot
    * Example mission to explore controlling two vessels in Gazebo using MOOS-IvP
