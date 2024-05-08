#ifndef HKGFileWriterBase_H
#define HKGFileWriterBase_H

#include <iostream>
#include <string>

#include <DataModel.h>
#include "Tool.h"

/**
 * \class HKGFileWriterBase
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 */

namespace HK {
	namespace Ghost {
		namespace IO {
			class HKGFileWriterBase : public Tool {

				public:

					HKGFileWriterBase();  ///< Simple constructor
					bool Initialise(
					    std::string configfile,
					    DataModel& data);  ///< Initialise Function for setting up Tool resources. @param
					///< configfile The path and name of the dynamic configuration file
					///< to read in. @param data A reference to the transient data
					///< class used to pass information between Tools.
					bool Execute();   ///< Execute function used to perform Tool purpose.
					bool Finalise();  ///< Finalise funciton used to clean up resources.

				private:

					/**
					 * Get the output filename.
					 * This can be automatically generated based on your GHOST toolchain configuration (run
					 * number, physics simulated, etc.). Or can be overridden using the override_filename
					 * option in your Writer tool config.
					 * @return True if success.
					 */
					virtual bool GetOutputFilename();
					/**
					 * Open the output file(s) and setup the output tree(s)/histogram(s)/...
					 * Called in Initalise().
					 * @return True if success.
					 */
					virtual bool SetupFile()            = 0;
					/**
					 * Fill the output tree(s)/histogram(s) that are event-independent (e.g. geometry).
					 * Called in Initalise().
					 * @return True if success.
					 */
					virtual bool FillEventIndependent() = 0;
					/**
					 * Fill the output tree(s)/histogram(s)/...
					 * Called in Execute().
					 * @return True if success.
					 */
					virtual bool FillThisEvent()        = 0;
					/**
					 * Write the output tree(s)/histogram(s)/... to file.
					 * Called in Finalise().
					 * @return True if success.
					 */
					virtual bool WriteFile()            = 0;
					/**
					 * Cleanup the memory used by this tool.
					 * Called in Finalise().
					 * @return True if success.
					 */
					virtual bool Cleanup()              = 0;

				protected:

					/// Output filename
					std::string m_filename;
			};
		}  // namespace IO
	}      // namespace Ghost
}  // namespace HK

#endif
