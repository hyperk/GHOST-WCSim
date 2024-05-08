#include "HKG4StackingAction.h"

using namespace HK::Ghost::G4;

HKG4StackingAction::HKG4StackingAction() : Tool() {}

bool HKG4StackingAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_p_UI = G4UImanager::GetUIpointer();
	m_data->m_p_run_manager->SetUserAction(
	    new WCSimStackingAction(static_cast<const WCSimDetectorConstruction*>(
	        m_data->m_p_run_manager->GetUserDetectorConstruction())));

	return true;
}

bool HKG4StackingAction::Execute() {

	return true;
}

bool HKG4StackingAction::Finalise() {

	return true;
}
