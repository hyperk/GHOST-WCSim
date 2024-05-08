#include "HKG4EventAction.h"

#include "WCSimDetectorConstruction.hh"
#include "WCSimEventAction.hh"
#include "WCSimPrimaryGeneratorAction.hh"
#include "WCSimRunAction.hh"

using namespace HK::Ghost::G4;

HKG4EventAction::HKG4EventAction() : Tool() {}

bool HKG4EventAction::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	const WCSimRunAction* cp_run =
	    static_cast<const WCSimRunAction*>(m_data->m_p_run_manager->GetUserRunAction());
	const WCSimDetectorConstruction* cp_det =
	    static_cast<const WCSimDetectorConstruction*>(m_data->m_p_run_manager->GetUserDetectorConstruction());
	const WCSimPrimaryGeneratorAction* cp_gen = static_cast<const WCSimPrimaryGeneratorAction*>(
	    m_data->m_p_run_manager->GetUserPrimaryGeneratorAction());

	m_data->m_p_run_manager->SetUserAction(new WCSimEventAction(cp_run, cp_det, cp_gen));

	return true;
}

bool HKG4EventAction::Execute() {

	return true;
}

bool HKG4EventAction::Finalise() {

	return true;
}
