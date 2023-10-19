/***************************************************************/                                       
/*    NAME: Michael DeFilippo                                  */                                       
/*    ORGN: Dept of Mechanical Engineering, MIT, Cambridge MA  */                                       
/*    FILE: ParseGateway.h                                     */                                       
/*    DATE: 2022-11-14                                         */                                       
/* This is unreleased BETA code. no permission is granted or   */                                       
/* implied to use, copy, modify, and distribute this software  */                                       
/* except by the author(s), or those designated by the author. */                                       
/***************************************************************/ 

#ifndef ParseGateway_HEADER
#define ParseGateway_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOSGeodesy/MOOSGeodesy.h"
#include "XYConvexGrid.h"                                                                               
#include "XYFormatUtilsConvexGrid.h"

class ParseGateway : public AppCastingMOOSApp
{
 public:
   ParseGateway();
   ~ParseGateway();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();  

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

protected:
  void registerVariables();
  bool GeodesySetup();
  void handleGazeboNodeReport(std::string report);
  void handleWptUpdate(std::string wpt_update);
  void handleStationingUpdate(std::string station_update);
  

 private: // Configuration variables

 private: // State variables
  std::string m_last_rcvd;
  std::string m_last_node_report;

  double m_nav_x;
  double m_nav_y;
  double m_heading;
  double m_lat;
  double m_lon;
  double m_yaw;
  double m_pitch;
  double m_roll;
  double m_speed;
  double m_depth;
  double m_update_x;
  double m_update_y;
  double m_update_lat;
  double m_update_lon;
  double m_station_x;
  double m_station_y;
  double m_station_lat;
  double m_station_lon;
  double m_station_radius;

  // Geodesy related
  CMOOSGeodesy m_Geodesy;
  double m_LatOrigin;
  double m_LonOrigin;


};

#endif 
