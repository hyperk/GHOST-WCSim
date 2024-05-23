#ifndef WCSim_exe_H
#define WCSim_exe_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

// #include "G4RunManager.hh"
#include "G4UImanager.hh"

/**
 * \class WCSim_exe
 *
 * Tool to take the role of WCSim.cc
 *
 * $Author: T.Dealtry $
 * $Date: 2023/08/08 14:31:00 $
 */

namespace Ghost::G4 {
	class WCSim_exe : public Tool {

		public:

			WCSim_exe();  ///< Simple constructor
			bool Initialise(std::string configfile,
			                DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
			///< configfile The path and name of the dynamic configuration file
			///< to read in. @param data A reference to the transient data
			///< class used to pass information between Tools.
			bool Execute();   ///< Execute function used to perform Tool purpose.
			bool Finalise();  ///< Finalise funciton used to clean up resources.

		private:

			// G4RunManager* m_p_run_manager;
			G4UImanager* m_p_UI;
			bool m_initialised;
			std::string m_wcsim_mac_filename;
			long m_number_of_events;
			long m_current_event;
	};
}  // namespace Ghost::G4
#endif
