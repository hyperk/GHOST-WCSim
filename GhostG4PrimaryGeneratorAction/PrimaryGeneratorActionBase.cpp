#include "PrimaryGeneratorActionBase.h"

Ghost::G4::PrimaryGeneratorActionBase::PrimaryGeneratorActionBase() :
    m_generator_name("UNKNOWNGENERATOR"), m_event_num(0), m_vertex_time_unit(CLHEP::nanosecond) {
	m_particle_gun = std::make_unique<G4ParticleGun>();
}

Ghost::G4::PrimaryGeneratorActionBase::~PrimaryGeneratorActionBase() {}

void Ghost::G4::PrimaryGeneratorActionBase::GeneratePrimaries(G4Event* anEvent) {
	G4cout << "Generating primary particles from generator: " << m_generator_name << " for event "
	       << m_event_num << G4endl;

	ClearParticleVectors();
	FillParticleVectors();

	G4cout << "There are " << GetNParticlesToBeSimulated() << " (" << GetNParticlesToBeSaved() << ")"
	       << " particles to be simulated (saved)" << G4endl;

	for(const InitialParticleInfo& particle : m_particles_to_save) {
		G4cerr << "Need to implement saving particles" << G4endl;
	}  // loop over particles to save

	for(const InitialParticleInfo& particle : m_particles_to_simulate) {
		m_particle_gun->SetParticleDefinition(particle.m_p_particle_definition);
		m_particle_gun->SetParticleTime(particle.m_time * m_vertex_time_unit);
		m_particle_gun->SetParticleEnergy(particle.m_kinetic_energy);
		m_particle_gun->SetParticleMomentumDirection(particle.m_direction);
		m_particle_gun->SetParticlePosition(particle.m_position);

		m_particle_gun->GeneratePrimaryVertex(anEvent);
	}  // loop over particles to simulate
}
