#include "MyTool.h"

GhostNAMESPACE::MyTool::MyTool() : Tool() {}

bool GhostNAMESPACE::MyTool::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	return true;
}

bool GhostNAMESPACE::MyTool::Execute() {

	return true;
}

bool GhostNAMESPACE::MyTool::Finalise() {

	return true;
}
