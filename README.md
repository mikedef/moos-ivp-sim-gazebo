# MOOS-IvP-Sim-Gazebo

This repo contains the basic autonomy interface for controlling a simulated vessel in Gazebo. This repo is designed to be used with moos-ivp-gateway <link>. Be sure to source each moos-ivp tree properly in .bashrc. This repo is also intended to be used with the ROS package gazebo_sim_moos_ivp <link>. 

# Src
  * iGazeboVessel
    * An interface to control a simulated Gazebo vessel from the MOOS-IvP pMarinePID `DESIRED_THRUST` and `DESIRED_RUDDER` parameters

  * pParseGateway
    * A basic parsing application for ingesting simulated vessel sensor data from Gazebo
    * Copy into your project (moos-ivp-<project>/src) and edit as needed
