#include "HKGFileWriterRootWCSim.h"

using namespace HK::Ghost::IO;

HKGFileWriterRootWCSim::HKGFileWriterRootWCSim() : HKGFileWriterBase() {
	m_p_file       = nullptr;
	m_p_tree_event = nullptr;
	m_p_tree_geom  = nullptr;
}

bool HKGFileWriterRootWCSim::SetupFile() {
	// Create the file
	m_p_file = new TFile(m_filename.c_str(), "RECREATE");
	if(m_p_file == nullptr)
		return false;

	// Create & setup the event tree
	m_p_tree_event = new TTree("wcsimT", "WCSim Event tree");

	// Create & setup the geometry tree
	m_p_tree_geom = new TTree("wcsimGeoT", "WCSim geometry tree");

	return true;
}

bool HKGFileWriterRootWCSim::FillEventIndependent() {
	if(m_p_tree_geom->Fill() < 0)
		return false;
	return true;
}

bool HKGFileWriterRootWCSim::FillThisEvent() {
	if(m_p_tree_event->Fill() < 0)
		return false;
	return true;
}

bool HKGFileWriterRootWCSim::WriteFile() {
	m_p_file->cd();
	m_p_file->Write();
	return true;
}

bool HKGFileWriterRootWCSim::Cleanup() {
	delete m_p_tree_geom;
	delete m_p_tree_event;
	delete m_p_file;
	return true;
}
