#!/bin/bash -e
#----------------------------------------------------------
#  Script: launch.sh
#  Author: Michael DeFilippo addapted from Michael Benjamin
#  LastEd: 2023
#----------------------------------------------------------
#  Part 1: Set Exit actions and declare global var defaults
#----------------------------------------------------------
trap "kill -- -$$" EXIT SIGTERM SIGHUP SIGINT SIGKILL
TIME_WARP=1
JUST_MAKE="no"

#-------------------------------------------------------
#  Part 2: Check for and handle command-line arguments
#-------------------------------------------------------
for ARGI; do
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then
	echo "launch.sh [SWITCHES] [time_warp]   " 
	echo "  --just_make, -j                  " 
	echo "  --help, -h                       " 
	exit 0;
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI}" = "--just_make" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    else 
        echo "launch.sh Bad arg:" $ARGI " Exiting with code: 1"
        exit 1
    fi
done

#-------------------------------------------------------
#  Part 3: Create the .moos and .bhv files. 
#-------------------------------------------------------
VNAME1="ASSET_1"

nsplug meta_asset1.moos targ_"$VNAME1".moos -f WARP=$TIME_WARP       \
   VNAME=$VNAME1   VPORT="9001"         GPORT="9501"                               \
   VTYPE="ship"    SHARE_LISTEN="9301"                        \

nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
   SNAME="shoreside"  SHARE_LISTEN="9300"                     \
   SPORT="9000" 

#-------------------------------------------------------   
nsplug meta_asset1.bhv targ_$VNAME1.bhv -f VNAME=$VNAME1        

if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 4: Launch the processes
#-------------------------------------------------------

echo "Launching $VNAME1 MOOS Community with WARP:" $TIME_WARP
pAntler targ_$VNAME1.moos >& /dev/null &
sleep 0.25
echo "Launching $SNAME MOOS Community with WARP:"  $TIME_WARP                                           
pAntler targ_shoreside.moos >& /dev/null &                                                              
sleep 0.25
echo "Done "

uMAC targ_shoreside.moos
