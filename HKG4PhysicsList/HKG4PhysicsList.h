#ifndef HKG4PhysicsList_H
#define HKG4PhysicsList_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

#include "G4UImanager.hh"

#include "WCSimPhysicsListFactory.hh"

/**
 * \class HKG4PhysicsList
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 */

namespace Ghost::G4 {

	class HKG4PhysicsList : public Tool {

		public:

			HKG4PhysicsList();  ///< Simple constructor
			bool Initialise(std::string configfile,
			                DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
			///< configfile The path and name of the dynamic configuration file
			///< to read in. @param data A reference to the transient data
			///< class used to pass information between Tools.
			bool Execute();   ///< Execute function used to perform Tool purpose.
			bool Finalise();  ///< Finalise funciton used to clean up resources.

		private:

			WCSimPhysicsListFactory* m_p_phys_factory;
			G4UImanager* m_p_UI;
	};
}  // namespace Ghost::G4

#endif
