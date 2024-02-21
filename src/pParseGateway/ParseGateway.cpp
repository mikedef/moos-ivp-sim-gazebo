/***************************************************************/
/*    NAME: Michael DeFilippo                                  */
/*    ORGN: Dept of Mechanical Engineering, MIT, Cambridge MA  */
/*    FILE: ParseGateway.cpp                                   */
/*    DATE: 2022-11-14                                         */
/*    NOTE: Basic parsing application for ingesting simulated  */
/*          vessel sensor data from Gazebo                     */
/* Copyrght MIT and author/s of software.                      */
/* This is unreleased BETA code. no permission is granted or   */
/* implied to use, copy, modify, and distribute this software  */
/* except by the author(s), or those designated by the author. */
/***************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "ParseGateway.h"

using namespace std;

//---------------------------------------------------------
// Constructor()

ParseGateway::ParseGateway()
{

}

//---------------------------------------------------------
// Destructor

ParseGateway::~ParseGateway()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool ParseGateway::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    string sval  = msg.GetString();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    // Handle Gazebo Vessel Node Report
    if(key == "ASSET_NODE_REPORT_GAZEBO"){
      handleGazeboNodeReport(sval);
    }

    // Send waypoint update
    else if(key == "WAYPOINT_UPDATE"){  // change to GAZEBO_MOVE_TO
      handleWptUpdate(sval);
    }

    // Send station keep update
    else if(key == "STATION_KEEP_UPDATE"){
      handleStationingUpdate(sval);
    }

    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }

   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool ParseGateway::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool ParseGateway::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: handleNodeReportGazebo
// Handle vessel node report from Gazebo
//
void ParseGateway::handleGazeboNodeReport(std::string report)
{
  // Parse data
  vector<string> str_vector = parseStringQ(report, ',');
  string name = tokStringParse(report, "NAME", ',',  '=');
  m_lat = stod( tokStringParse(report, "LAT", ',',  '=') );
  m_lon = stod( tokStringParse(report, "LON", ',',  '=') );
  m_speed = stod( tokStringParse(report, "SPD", ',',  '=') );
  m_depth = stod( tokStringParse(report, "DEP", ',',  '=') );
  m_heading = stod( tokStringParse(report, "HDG", ',',  '=') );
  m_roll = stod( tokStringParse(report, "ROLL", ',',  '=') );
  m_pitch = stod( tokStringParse(report, "PITCH", ',',  '=') );

  m_Geodesy.LatLong2LocalUTM(m_lat, m_lon, m_nav_y, m_nav_x);

  m_last_rcvd = report;
  m_last_node_report = str_vector[0];

  // Post Report
  //Notify("NODE_REPORT_LOCAL", report); // Not needed. Helm is looking for X/Y
  Notify("NAV_LAT", m_lat);
  Notify("NAV_LON", m_lon);
  Notify("NAV_LONG", m_lon);
  Notify("NAV_HEADING", m_heading);
  Notify("NAV_SPEED", m_speed);
  Notify("NAV_DEPTH", m_depth);
  Notify("NAV_ROLL", m_roll);
  Notify("NAV_PITCH", m_pitch);
  Notify("NAV_X", m_nav_x);
  Notify("NAV_Y", m_nav_y);

}

//---------------------------------------------------------
// Procedure: handleWptUpdate()
// TODO: Update to handle single wpt to multiple point updates
//
void ParseGateway::handleWptUpdate(std::string wpt_update)
{
  // Parse Data:
  m_update_lat = stod( tokStringParse(wpt_update, "LAT", ',', '=') );
  m_update_lon = stod( tokStringParse(wpt_update, "LONG", ',', '=') );

  // Transform to local
  m_Geodesy.LatLong2LocalUTM(m_update_lat, m_update_lon, m_update_y, m_update_x);

  // Post Waypoint Update
  m_last_rcvd = wpt_update;
  std::string point = "point=" + to_string(m_update_x) + ',' + to_string(m_update_y);
  Notify("WPT_UPDATE" , true);  // Enter into MODE:TRANSITING
  Notify("WPT_UPDATES", point);
}

//---------------------------------------------------------
// Procedure: handleStationingUpdate()
//
void ParseGateway::handleStationingUpdate(std::string station_update)
{
  // Parse Data
  m_station_lat = stod( tokStringParse(station_update, "LAT", ',', '=') );
  m_station_lon = stod( tokStringParse(station_update, "LONG", ',', '=') );
  m_station_radius = stod( tokStringParse(station_update, "RADIUS", ',', '=') );

  // Transform to Local
  m_Geodesy.LatLong2LocalUTM(m_station_lat, m_station_lon, m_station_y, m_station_x);

  // Post Station Keep Update
  m_last_rcvd = station_update;
  std::string updates = "station_pt=" + to_string(m_station_x) + ',' + to_string(m_station_y)
    + " # center_activate=false" + " # inner_radius=" + to_string(m_station_radius - 10)
    + " # outer_radius=" + to_string(m_station_radius);
  Notify("STATION_REQUEST", "true");  // Conditional to acivate BHV
  Notify("DEPLOY", "false");
  Notify("RETURN", "false");
  Notify("STATION_UPDATES", updates);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool ParseGateway::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      //Register(line, 0);
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }

  GeodesySetup();
  registerVariables();
  return(true);
}

//--------------------------------------------------------
// Procedure: GeodesySetup()
bool ParseGateway::GeodesySetup()
{
  // Get Latitude Origin from .MOOS Mission File
  bool latOK = m_MissionReader.GetValue("LatOrigin", m_LatOrigin);
  if(!latOK) {
    reportConfigWarning("Latitude origin missing in MOOS file.");
    return(false);
  }

  // Get Longitude Origin from .MOOS Mission File
  bool lonOK = m_MissionReader.GetValue("LongOrigin", m_LonOrigin);
  if(!lonOK){
    reportConfigWarning("Longitude origin missing in MOOS file.");
    return(false);
  }

  // Initialise CMOOSGeodesy object
  bool geoOK = m_Geodesy.Initialise(m_LatOrigin, m_LonOrigin);
  if(!geoOK) {
    reportConfigWarning("Geodesy::Initialise() failed. Invalid origin.");
    return(false);
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables()

void ParseGateway::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("ASSET_NODE_REPORT_GAZEBO", 0);
  Register("WAYPOINT_UPDATE", 0);
  Register("STATION_KEEP_UPDATE", 0);

}


//------------------------------------------------------------
// Procedure: buildReport()

bool ParseGateway::buildReport()
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(2);
  //actab << "Alpha | Bravo | Charlie | Delta";
  //actab.addHeaderLines();
  actab << "last received msg: " << m_last_rcvd;
  actab << "lat: " << to_string(m_lat);
  actab << "lon: " << to_string(m_lon);
  actab << "nav_x: " << to_string(m_nav_x);
  actab << "nav_y: " << to_string(m_nav_y);
  actab << "update_x: " << to_string(m_update_x);
  actab << "update_y: " << to_string(m_update_y);
  m_msgs << actab.getFormattedString();

  return(true);
}




