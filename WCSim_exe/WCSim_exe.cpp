#include "WCSim_exe.h"

#include "WCSimDetectorConstruction.hh"
#include "WCSimEventAction.hh"
#include "WCSimPhysicsListFactory.hh"
#include "WCSimPrimaryGeneratorAction.hh"
#include "WCSimRandomParameters.hh"
#include "WCSimRunAction.hh"
#include "WCSimStackingAction.hh"
#include "WCSimSteppingAction.hh"
// #include "WCSimTrackingAction.hh"
#include "WCSimTuningParameters.hh"

#include "../src/Utilities.h"

using namespace HK::Ghost::G4;

WCSim_exe::WCSim_exe() : Tool() {}

bool WCSim_exe::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	///////////
	// GET ALL THE PARAMETERS FROM THE CONFIG FILE
	///////////
	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	// get all the mac files
	std::string wcsim_dir = HK::Ghost::utils::GetEnvironmentVariableWithDefault("WCSIM_BUILD_DIR", "./");
	/*std::string wcsim_mac_job_opt_filename =
	    HK::Ghost::utils::GetConfigFilename(m_variables, "wcsim_mac_job_opt_filename", (wcsim_dir +
	   "/macros/jobOptions.mac").c_str());
	*/
	std::string wcsim_mac_tuning_filename =
	    HK::Ghost::utils::GetConfigFilename(m_variables,
	                                        "wcsim_mac_tuning_filename",
	                                        (wcsim_dir + "/macros/tuning_parameters.mac").c_str());
	m_wcsim_mac_filename = HK::Ghost::utils::GetConfigFilename(m_variables,
	                                                           "wcsim_mac_filename",
	                                                           (wcsim_dir + "/WCSim.mac").c_str());

	// get the number of events
	if(!m_variables.Get("number_of_events", m_number_of_events)) {
		std::cout << "number_of_events not found in config file. Just simulating 1 event" << std::endl;
		m_number_of_events = 1;
	}
	// initialise the current event number
	m_current_event = 0;

	///////////
	// SETUP GEANT4
	///////////

	// Construct the default run manager
	m_data->m_p_run_manager = new G4RunManager;

	// get the pointer to the UI manager
	m_p_UI = G4UImanager::GetUIpointer();

	// Set up the tuning parameters that need to be read before the detector
	//  construction is done
	WCSimTuningParameters* tuningpars = new WCSimTuningParameters();
	std::cout << "Processing tuning parameter file " << wcsim_mac_tuning_filename << std::endl;
	m_p_UI->ApplyCommand("/control/execute " + wcsim_mac_tuning_filename);

	// define random number generator parameters
	// WCSimRandomParameters* randomparameters = new WCSimRandomParameters();

	// UserInitialization classes (mandatory)
	enum DetConfiguration { wfm = 1, fwm = 2 };

	G4int WCSimConfiguration = fwm;

	WCSimDetectorConstruction* WCSimdetector = new WCSimDetectorConstruction(WCSimConfiguration, tuningpars);

	m_data->m_p_run_manager->SetUserInitialization(WCSimdetector);

	*m_log << ML(0) << "TODO: Add tuningpars->SaveOptionsToOutput(myRunAction->GetRootOptions());" << endl;

	// Set user action classes
	// WCSimPrimaryGeneratorAction* myGeneratorAction = new WCSimPrimaryGeneratorAction(WCSimdetector);
	// m_data->m_p_run_manager->SetUserAction(myGeneratorAction);

	// WCSimRunAction* myRunAction = new WCSimRunAction(WCSimdetector, randomparameters);

	// save all the options from WCSimTuningParameters & WCSimPhysicsListFactory
	//(set in tuning_parameters.mac & jobOptions*.mac)
	// tuningpars->SaveOptionsToOutput(myRunAction->GetRootOptions());
	// physFactory->SaveOptionsToOutput(myRunAction->GetRootOptions());

	// m_data->m_p_run_manager->SetUserAction(myRunAction);

	// m_data->m_p_run_manager->SetUserAction(new WCSimEventAction(myRunAction, WCSimdetector,
	// myGeneratorAction)); m_data->m_p_run_manager->SetUserAction(new WCSimTrackingAction);

	// m_data->m_p_run_manager->SetUserAction(new WCSimStackingAction(WCSimdetector));

	// m_data->m_p_run_manager->SetUserAction(new WCSimSteppingAction(myRunAction, WCSimdetector));

	m_initialised = false;

	return true;
}

bool WCSim_exe::Execute() {

	if(!m_initialised) {
		// Initialize G4 kernel
		m_data->m_p_run_manager->Initialize();

		m_p_UI->ApplyCommand("/control/execute " + m_wcsim_mac_filename);

		m_initialised = true;
	}

	if(m_current_event < m_number_of_events) {
		m_p_UI->ApplyCommand("/run/beamOn 1");
	}
	m_current_event++;
	if(m_current_event >= m_number_of_events)
		m_data->vars.Set("StopLoop", 1);
	return true;
}

bool WCSim_exe::Finalise() {
	delete m_data->m_p_run_manager;

	return true;
}
