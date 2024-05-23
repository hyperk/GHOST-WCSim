#ifndef InitialParticleInfo_h
#define InitialParticleInfo_h

#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"

namespace Ghost::G4 {
	struct InitialParticleInfo {
			unsigned int m_vertex_num;
			double m_time;
			double m_kinetic_energy;
			G4ThreeVector m_direction;
			G4ThreeVector m_position;
			G4ParticleDefinition* m_p_particle_definition;
			int m_particle_pdg;
			int m_particle_charge;

			//! Use this constructor if you don't require telling Geant4 about your particle
			InitialParticleInfo(unsigned int vertex_num,
			                    double time,
			                    double kinetic_energy,
			                    G4ThreeVector direction,
			                    G4ThreeVector position,
			                    int pdg) :
			    m_vertex_num(vertex_num),
			    m_time(time),
			    m_kinetic_energy(kinetic_energy),
			    m_direction(direction),
			    m_position(position),
			    m_p_particle_definition(nullptr),
			    m_particle_pdg(pdg),
			    m_particle_charge(INT_MAX) {}

			//! Use this constructor if you need to tell Geant4 about your particle
			InitialParticleInfo(unsigned int vertex_num,
			                    double time,
			                    double kinetic_energy,
			                    G4ThreeVector direction,
			                    G4ThreeVector position,
			                    G4ParticleDefinition* particle_pdg) :
			    m_vertex_num(vertex_num),
			    m_time(time),
			    m_kinetic_energy(kinetic_energy),
			    m_direction(direction),
			    m_position(position),
			    m_p_particle_definition(particle_pdg),
			    m_particle_charge(INT_MAX) {
				m_particle_pdg = m_p_particle_definition->GetPDGEncoding();
			}

			~InitialParticleInfo() {}
	};
}  // namespace Ghost::G4

#endif
