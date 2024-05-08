#include "HKG4RunAction.h"

#include "WCSimRandomParameters.hh"
#include "WCSimRunAction.hh"

using namespace HK::Ghost::G4;

HKG4RunAction::HKG4RunAction() : Tool() {}

bool HKG4RunAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	// define random number generator parameters
	WCSimRandomParameters* randomparameters = new WCSimRandomParameters();

	WCSimRunAction* myRunAction = new WCSimRunAction(
	    static_cast<const WCSimDetectorConstruction*>(m_data->m_p_run_manager->GetUserDetectorConstruction()),
	    randomparameters);
	m_data->m_p_run_manager->SetUserAction(myRunAction);

	return true;
}

bool HKG4RunAction::Execute() {

	return true;
}

bool HKG4RunAction::Finalise() {

	return true;
}
