#include "PrimaryGeneratorAction.hh"


namespace B4
{

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction()
{
    theParticleSource = new G4GeneralParticleSource();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete theParticleSource;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
//   // Set gun position
//   fParticleGun
//    ->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    
    theParticleSource->GeneratePrimaryVertex(anEvent);
}

}
