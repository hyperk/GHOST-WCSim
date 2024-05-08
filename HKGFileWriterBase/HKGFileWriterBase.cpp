#include "HKGFileWriterBase.h"

#include <sstream>

using namespace HK::Ghost::IO;

HKGFileWriterBase::HKGFileWriterBase() : Tool() {}

bool HKGFileWriterBase::Initialise(std::string configfile, DataModel& data) {

	if(configfile != "")
		m_variables.Initialise(configfile);
	// m_variables.Print();

	m_data = &data;
	m_log  = m_data->Log;

	if(!m_variables.Get("verbose", m_verbose))
		m_verbose = 1;

	if(!GetOutputFilename()) {
		*m_log << ML(0) << "GetOutputFilename() did not succeed. Stopping toolchain" << std::endl;
		return false;
	}
	if(!SetupFile()) {
		*m_log << ML(0) << "SetupFile() did not succeed. Stopping toolchain" << std::endl;
		return false;
	}
	if(!FillEventIndependent()) {
		*m_log << ML(0) << "FillEventIndependent() did not succeed. Stopping toolchain" << std::endl;
		return false;
	}

	return true;
}

bool HKGFileWriterBase::GetOutputFilename() {
	// Override the automatic filename construction
	if(!m_variables.Get("override_filename", m_filename)) {

		// Setup the filename automatically
		std::stringstream ss;
		ss << "ghost_"
		   << "RUN"
		   << "_"
		   << "SUBRUN"
		   << "_"
		   << "PHYSICS"
		   << ".root";
		m_filename = ss.str();

		*m_log << ML(1) << "TODO build auto filename from things stored in the DataModel" << std::endl;
	}
	*m_log << ML(2) << "Using output filename: " << m_filename << std::endl;
	return true;
}

bool HKGFileWriterBase::Execute() {

	if(!FillThisEvent()) {
		*m_log << ML(0) << "FillThisEvent() did not succeed. Stopping toolchain" << std::endl;
		return false;
	}
	return true;
}

bool HKGFileWriterBase::Finalise() {

	if(!WriteFile()) {
		*m_log << ML(0) << "WriteFile() did not succeed. Stopping toolchain" << std::endl;
		return false;
	}
	if(!Cleanup()) {
		*m_log << ML(0) << "Cleanup() did not succeed. Stopping toolchain" << std::endl;
		return false;
	}
	return true;
}
