#include "HKG4PrimaryGeneratorAction.h"

#include "WCSimPrimaryGeneratorAction.hh"

using namespace HK::Ghost::G4;

HKG4PrimaryGeneratorAction::HKG4PrimaryGeneratorAction() : Tool() {}

bool HKG4PrimaryGeneratorAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	m_data->m_p_run_manager->SetUserAction(
	    new WCSimPrimaryGeneratorAction(static_cast<const WCSimDetectorConstruction*>(
	        m_data->m_p_run_manager->GetUserDetectorConstruction())));

	return true;
}

bool HKG4PrimaryGeneratorAction::Execute() {

	return true;
}

bool HKG4PrimaryGeneratorAction::Finalise() {

	return true;
}
