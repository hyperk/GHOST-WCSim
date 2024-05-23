#ifndef MYTOOL_H
#define MYTOOL_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

/**
 * \class MyTool
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 */

namespace GhostNAMESPACE {
	class MyTool : public Tool {

		public:

			//! Simple constructor
			MyTool();

			//! Initialise Function for setting up Tool resources.
			/**
			 * @param
			 * configfile The path and name of the dynamic configuration file
			 * to read in. @param data A reference to the transient data
			 * class used to pass information between Tools.
			 */
			bool Initialise(std::string configfile, DataModel& data);
			//! Execute function used to perform Tool purpose.
			bool Execute();
			//! Finalise funciton used to clean up resources.
			bool Finalise();

		private:
	};
}  // namespace GhostNAMESPACE

#endif
