##############################################################################
# FILE:        moos-ivp-sim-gazebo/src/CMakeLists.txt
# DATE:        2023/08/08
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-sim-gazebo source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
#ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(pParseGateway)
ADD_SUBDIRECTORY(iGazeboVessel)

##############################################################################
#                           END of CMakeLists.txt
##############################################################################

