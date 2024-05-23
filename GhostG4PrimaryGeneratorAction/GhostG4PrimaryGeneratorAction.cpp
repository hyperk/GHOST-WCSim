#include "GhostG4PrimaryGeneratorAction.h"
#include "PGAKinFile.h"

#include "WCSimPrimaryGeneratorAction.hh"

Ghost::G4::GhostG4PrimaryGeneratorAction::GhostG4PrimaryGeneratorAction() : Tool() {}

bool Ghost::G4::GhostG4PrimaryGeneratorAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	if(0)
		m_data->m_p_run_manager->SetUserAction(
	    new WCSimPrimaryGeneratorAction(static_cast<const WCSimDetectorConstruction*>(
																																										m_data->m_p_run_manager->GetUserDetectorConstruction())));
	else {		
		m_data->m_p_run_manager->SetUserAction(new Ghost::G4::PGAKinFile());
	}
	return true;
}

bool Ghost::G4::GhostG4PrimaryGeneratorAction::Execute() {

	return true;
}

bool Ghost::G4::GhostG4PrimaryGeneratorAction::Finalise() {

	return true;
}
