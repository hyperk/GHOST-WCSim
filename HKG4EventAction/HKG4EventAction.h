#ifndef HKG4EventAction_H
#define HKG4EventAction_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

/**
 * \class HKG4EventAction
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 *
 * $Author: B.Richards $
 * $Date: 2019/05/28 10:44:00 $
 */

namespace HK {
	namespace Ghost {
		namespace G4 {
			class HKG4EventAction : public Tool {

				public:

					HKG4EventAction();  ///< Simple constructor
					bool Initialise(
					    std::string configfile,
					    DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
					///< configfile The path and name of the dynamic configuration file
					///< to read in. @param data A reference to the transient data
					///< class used to pass information between Tools.
					bool Execute();   ///< Execute function used to perform Tool purpose.
					bool Finalise();  ///< Finalise funciton used to clean up resources.

				private:
			};
		}  // namespace G4
	}      // namespace Ghost
}  // namespace HK

#endif
