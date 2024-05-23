#ifndef PrimaryGeneratorActionBase_h
#define PrimaryGeneratorActionBase_h

#include "G4ParticleGun.hh"
#include "G4String.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

#include "InitialParticleInfo.h"

#include "WCSimRootOptions.hh"

namespace Ghost::G4 {

	class PrimaryGeneratorActionBase : public G4VUserPrimaryGeneratorAction {
		public:

			PrimaryGeneratorActionBase();
			~PrimaryGeneratorActionBase();

			//! Generates primary to simulate through Geant4
			virtual void GeneratePrimaries(G4Event* anEvent);

			//! Get the number of particles to be simulated in this event
			size_t GetNParticlesToBeSimulated() const { return m_particles_to_simulate.size(); }

			//! Get the number of particles to be saved in this event
			size_t GetNParticlesToBeSaved() const { return m_particles_to_save.size(); }

			//! Get the generator name
			G4String GetGeneratorTypeString() const { return m_generator_name; }

		protected:

			//! Add a particle to the list of initial particles that Geant4 will simulate
			void AddParticleToSimulate(InitialParticleInfo& pinfo) {
				m_particles_to_simulate.push_back(pinfo);
			}

			//! Add a particle to the list of initial particles that Geant4 won't simulate
			void AddParticleToSave(InitialParticleInfo& pinfo) { m_particles_to_save.push_back(pinfo); }

			//! Build you event here. This should be calls of AddParticleToSimulate and AddParticleToSave
			virtual void FillParticleVectors() = 0;

			//! Store the particles we want to simulate with Geant4
			std::vector<InitialParticleInfo> m_particles_to_simulate;

			//! Store any particles we want to save, but not simulate
			/**
			 * e.g. Beam, target, pre-FSI
			 */
			std::vector<InitialParticleInfo> m_particles_to_save;

			//! The name of the generator. Will be replaced with an enum
			[[deprecated]] G4String m_generator_name;

			//! Store config info in the WCSimRootOptions. Will be replaced with a common way to store things
			//! in the DataModel
			[[deprecated]] virtual void SaveOptionsToOutput(WCSimRootOptions* wcopt) const = 0;

			//! Event number counter.
			int m_event_num;

			//! The time unit we're using.
			G4double m_vertex_time_unit;

		private:

			//! How we actually generate particles to simulate through Geant4
			std::unique_ptr<G4ParticleGun> m_particle_gun;

			void ClearParticleVectors() {
				m_particles_to_simulate.clear();
				m_particles_to_save.clear();
			}

		public:

			//! Set the time unit
			void SetVertexTimeUnit(G4String choice) {
				if(choice == "ns" || choice == "nanosecond")
					m_vertex_time_unit = CLHEP::nanosecond;
				else if(choice == "s" || choice == "second")
					m_vertex_time_unit = CLHEP::second;
				else if(choice == "ms" || choice == "millisecond")
					m_vertex_time_unit = CLHEP::millisecond;
				else if(choice == "microsecond")
					m_vertex_time_unit = CLHEP::microsecond;
				else if(choice == "ps" || choice == "picosecond")
					m_vertex_time_unit = CLHEP::picosecond;
				else
					m_vertex_time_unit = CLHEP::nanosecond;
			}

			//! Get the time unit
			G4double GetVertexTimeUnit() const { return m_vertex_time_unit; }
	};
}  // namespace Ghost::G4
#endif
