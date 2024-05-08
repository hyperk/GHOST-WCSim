#include "HKG4SteppingAction.h"

#include "WCSimSteppingAction.hh"

using namespace HK::Ghost::G4;

HKG4SteppingAction::HKG4SteppingAction() : Tool() {}

bool HKG4SteppingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_run_manager->SetUserAction(new WCSimSteppingAction(
	    static_cast<const WCSimRunAction*>(m_data->m_p_run_manager->GetUserRunAction()),
	    static_cast<const WCSimDetectorConstruction*>(
	        m_data->m_p_run_manager->GetUserDetectorConstruction())));

	return true;
}

bool HKG4SteppingAction::Execute() {

	return true;
}

bool HKG4SteppingAction::Finalise() {

	return true;
}
