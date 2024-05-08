#ifndef HKGFileWriterRootWCSim_H
#define HKGFileWriterRootWCSim_H

#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"

#include "HKGFileWriterBase.h"

#include <DataModel.h>
#include "Tool.h"

/**
 * \class HKGFileWriterRootWCSim
 *
 * This is a blank template for a Tool used by the script to generate a new custom tool. Please fill out the
 * description and author information.
 */

namespace HK {
	namespace Ghost {
		namespace IO {
			class HKGFileWriterRootWCSim : public HKGFileWriterBase {

				public:

					HKGFileWriterRootWCSim();  ///< Simple constructor

				private:

					/**
					 * Open the output file(s) and setup the output tree(s)/histogram(s)/...
					 * Called in Initalise().
					 * @return True if success.
					 */
					bool SetupFile() override;
					/**
					 * Fill the output tree(s)/histogram(s) that are event-independent (e.g. geometry).
					 * Called in Initalise().
					 * @return True if success.
					 */
					bool FillEventIndependent() override;
					/**
					 * Fill the output tree(s)/histogram(s)/...
					 * Called in Execute().
					 * @return True if success.
					 */
					bool FillThisEvent() override;
					/**
					 * Write the output tree(s)/histogram(s)/... to file.
					 * Called in Finalise().
					 * @return True if success.
					 */
					bool WriteFile() override;
					/**
					 * Cleanup the memory used by this tool.
					 * Called in Finalise().
					 * @return True if success.
					 */
					bool Cleanup() override;
			};

			/// Pointer to output file
			TFile* m_p_file;
			/// Pointer to event tree
			TTree* m_p_tree_event;
			/// Pointer to geometry tree
			TTree* m_p_tree_geom;
		}  // namespace IO
	}      // namespace Ghost
}  // namespace HK

#endif
