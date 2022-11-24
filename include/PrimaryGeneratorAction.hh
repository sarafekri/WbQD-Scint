#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

#include "DetectorConstruction.hh"


class G4Event;
class G4GeneralParticleSource;

namespace B4
{

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction( );
    ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event*);
    G4GeneralParticleSource* GetParticleSource() {return theParticleSource;};

private:
    G4GeneralParticleSource* theParticleSource;
};

}
