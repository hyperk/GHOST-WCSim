#ifndef PGAKinFile_h
#define PGAKinFile_h

#include "PrimaryGeneratorActionBase.h"

#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4String.hh"

#include "WCSimRootOptions.hh"
#include "jhfNtuple.h"

#include <fstream>

class G4ParticleGun;
class G4Event;

namespace Ghost::G4 {
	class PGAKinFile : public PrimaryGeneratorActionBase {
	public:
		PGAKinFile();
		~PGAKinFile();

		void FillParticleVectors() override;

		/*
		// Gun, laser & gps setting calls these functions to fill jhfNtuple and Root tree
		void SetVtx(G4ThreeVector i)     { vtxs[0] = i; nvtxs = 1; };
		void SetBeamEnergy(G4double i, G4int n = 0)   { beamenergies[n] = i;};
		void SetBeamDir(G4ThreeVector i, G4int n = 0) { beamdirs[n] = i;};
		void SetBeamPDG(G4int i, G4int n = 0)         { beampdgs[n] = i;};
		void SetNvtxs(G4int i)     { nvtxs = i; };
		void SetVtxs(G4int i, G4ThreeVector v)     { vtxs[i] = v; };

		// These go with jhfNtuple
		G4int GetVecRecNumber() const {return vecRecNumber;}
		G4int GetMode(int vertex = 0) const {return mode[vertex];};
		//InteractionType_t GetMode(int vertex = 0) {return mode[vertex];};
		G4int GetNvtxs() const {return nvtxs;};
		G4int GetVtxVol(G4int n = 0) const {return vtxsvol[n];};
		G4ThreeVector GetVtx(G4int n = 0) const {return vtxs[n];}
		G4double GetVertexTime(G4int n = 0) const {return vertexTimes[n];}
		G4int GetNpar() const {return npar;};
		G4int GetBeamPDG(G4int n = 0) const {return beampdgs[n];};
		G4double GetBeamEnergy(G4int n = 0) const {return beamenergies[n];};
		G4ThreeVector GetBeamDir(G4int n = 0) const {return beamdirs[n];};
		G4int GetTargetPDG(G4int n = 0) const {return targetpdgs[n];};
		G4double GetTargetEnergy(G4int n = 0) const {return targetenergies[n];};
		G4ThreeVector GetTargetDir(G4int n = 0) const {return targetdirs[n];};

		// older ...
		G4double GetNuEnergy() const {return nuEnergy;};
		G4double GetEnergy() const {return energy;};
		G4double GetXPos() const {return xPos;};
		G4double GetYPos() const {return yPos;};
		G4double GetZPos() const {return zPos;};
		G4double GetXDir() const {return xDir;};
		G4double GetYDir() const {return yDir;};
		G4double GetZDir() const {return zDir;};
		*/
			
		void SaveOptionsToOutput(WCSimRootOptions * wcopt) const override;

	private:
  
		std::fstream m_input_file;
		G4String m_input_filename;
		//! Maximum length of the input line + 1
		static const int m_line_size=1000;
		//! Buffer for reading the input file line by line
		char m_line_buffer[m_line_size];
	};
} //namespace Ghost::G4
#endif
