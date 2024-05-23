#include "PGAKinFile.h"
#include <algorithm>
#include <fstream>
#include <G4ios.hh>
#include <G4Types.hh>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include "G4Vector3D.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "WCSimDetectorConstruction.hh"
#include "WCSimPrimaryGeneratorMessenger.hh"

#include "Utilities.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

Ghost::G4::PGAKinFile::PGAKinFile() : m_input_filename("test.kin") {
	m_input_file = Ghost::Utils::OpenFileStream(m_input_filename);

	m_generator_name = "muline";
}

Ghost::G4::PGAKinFile::~PGAKinFile() {
	m_input_file.close();
}

void Ghost::G4::PGAKinFile::FillParticleVectors() {
	// We will need a particle table
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4IonTable* ionTable           = G4IonTable::GetIonTable();

	// The original documentation describing the nuance text format can be found here:
	// http://neutrino.phy.duke.edu/nuance-format/
	//
	// Information specific to WCSim can be found in the file Nuance_MC_Format.txt in
	// the doc/ directory.
	// The format must be strictly adhered to for it to be processed correctly.
	// The lines and their meanings from begin through info are fixed, and then
	// a variable number of tracks may follow.
	//
	std::vector<string> token = Ghost::Utils::readInLine(m_input_file, m_line_size, m_line_buffer);
	int iVertex               = 0;

	if(token.size() == 0 || token[0] == "stop") {
		G4cout << "End of nuance vector file - run terminated..." << G4endl;
		G4RunManager::GetRunManager()->AbortRun();
	}
	else if(token[0] != "begin") {
		G4cout << "unexpected line begins with \"" << token[0] << "\"we were expecting \" begin \"" << G4endl;
	}
	else {  // normal parsing begins here
		// Read the nuance line
		// should be nuance <value>
		// but could be just
		// nuance
		// if value is given set mode to equal it.

		token = Ghost::Utils::readInLine(m_input_file, m_line_size, m_line_buffer);
		while(token[0] == "nuance" && iVertex < MAX_N_VERTICES) {
			int VertexIntMode = 0;
			if(token.size() > 1)
				VertexIntMode = Ghost::Utils::atoi(token[1]);

			// Read the Vertex line
			// - this contains position and time
			token = Ghost::Utils::readInLine(m_input_file, m_line_size, m_line_buffer);
			G4ThreeVector VertexPosition(Ghost::Utils::atof(token[1]) * cm,
			                             Ghost::Utils::atof(token[2]) * cm,
			                             Ghost::Utils::atof(token[3]) * cm);
			G4double VertexTime = Ghost::Utils::atof(token[4]) * m_vertex_time_unit;

			// Next we read the incoming neutrino and target

			// First, the neutrino line
			token = Ghost::Utils::readInLine(m_input_file, m_line_size, m_line_buffer);

			int BeamPDG       = Ghost::Utils::atoi(token[1]);
			double BeamEnergy = Ghost::Utils::atof(token[2]) * MeV;
			G4ThreeVector BeamDirection(Ghost::Utils::atof(token[3]),
			                            Ghost::Utils::atof(token[4]),
			                            Ghost::Utils::atof(token[5]));
			G4cout << "Neutrino generated is = " << BeamPDG << ", Enu = " << BeamEnergy
			       << " and interacts through mode = " << VertexIntMode << G4endl;
			InitialParticleInfo beam(iVertex, VertexTime, BeamEnergy, BeamDirection, VertexPosition, BeamPDG);
			AddParticleToSave(beam);

			// Now read the target line(s)
			//  There can be some cases (2p2h i.e. neut mode = 2) where there are 2 targets. The while loop is
			//  added for this purpose.
			while(token = Ghost::Utils::readInLine(m_input_file, m_line_size, m_line_buffer),
			      token[0] == "track") {
				int TargetPDG       = Ghost::Utils::atoi(token[1]);
				double TargetEnergy = Ghost::Utils::atof(token[2]) * MeV;
				G4ThreeVector TargetDirection(Ghost::Utils::atof(token[3]),
				                              Ghost::Utils::atof(token[4]),
				                              Ghost::Utils::atof(token[5]));
				G4cout << "Target hit is = " << TargetPDG << ", E = " << TargetEnergy << G4endl;
				InitialParticleInfo target(iVertex,
				                           VertexTime,
				                           TargetEnergy,
				                           TargetDirection,
				                           VertexPosition,
				                           TargetPDG);
				AddParticleToSave(target);
			}  // loop over target lines

			// The info line is read in the exiting step of the while loop aboe
			// The info line is (almost) a dummy
			G4cout << "Vector File Record Number " << token[2] << G4endl;
			int VectorRecordNumber = Ghost::Utils::atoi(token[2]);

			// Now read the outgoing particles
			// These we will simulate.
			while(token = Ghost::Utils::readInLine(m_input_file, m_line_size, m_line_buffer),
			      token[0] == "track") {
				// We are only interested in the particles
				// that leave the nucleus, tagged by "0"
				if(token[6] == "0") {
					G4int pdgid           = Ghost::Utils::atoi(token[1]);
					G4double energy_total = Ghost::Utils::atof(token[2]) * MeV;
					G4ThreeVector dir     = G4ThreeVector(Ghost::Utils::atof(token[3]),
                                                      Ghost::Utils::atof(token[4]),
                                                      Ghost::Utils::atof(token[5]));

					G4ParticleDefinition* particle_definition = nullptr;
					int particle_charge                       = INT_MAX;
					// must handle the case of an ion seperatly from other particles
					// check PDG code if we have an ion.
					// PDG code format for ions ±10LZZZAAAI
					if(abs(pdgid) >= 1000000000) {
						// ion
						char strPDG[11];
						char strA[10] = { 0 };
						char strZ[10] = { 0 };
						long int A = 0, Z = 0;
						sprintf(strPDG, "%i", abs(pdgid));
						// stop GCC complaining about string truncation
						//  - we're copying from the middle of a long string
						//  - we do terminate the string correctly below
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
						strncpy(strZ, &strPDG[3], 3);
#pragma GCC diagnostic pop
						strncpy(strA, &strPDG[6], 3);
						strA[3]             = '\0';
						strZ[3]             = '\0';
						A                   = Ghost::Utils::atoi(strA);
						Z                   = Ghost::Utils::atoi(strZ);
						particle_definition = ionTable->GetIon(Z, A, 0.);
						particle_charge     = 0;
					}  // ion
					else {
						// not ion
						particle_definition = particleTable->FindParticle(pdgid);
					}  // not ion

					G4double mass = particle_definition->GetPDGMass();

					G4double ekin = energy_total - mass;

					G4cout << "Generating particle = " << pdgid << ", E = " << energy_total
					       << " MeV, Ec = " << ekin << " MeV, and dir = " << dir[0] << ", " << dir[1] << ", "
					       << dir[2] << G4endl;

					InitialParticleInfo particle(iVertex,
					                             VertexTime,
					                             ekin,
					                             dir,
					                             VertexPosition,
					                             particle_definition);
					AddParticleToSimulate(particle);
				}  // token[6] == "0" (particle exiting nucleus)
			}      // loop over "track" lines
			iVertex++;
			if(iVertex > MAX_N_VERTICES)
				G4cout << " CAN NOT DEAL WITH MORE THAN " << MAX_N_VERTICES
				       << " VERTICES - TRUNCATING EVENT HERE " << G4endl;
		}  // loop over vertex blocks
	}      // valid file format
}

void Ghost::G4::PGAKinFile::SaveOptionsToOutput(WCSimRootOptions* wcopt) const {
	wcopt->SetVectorFileName(m_input_filename);
	wcopt->SetGeneratorType(GetGeneratorTypeString());
}
