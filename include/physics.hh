#ifndef PHYSICS_HH
#define PHYSICS_HH    //using the same method (ifndef and define)

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmPenelopePhysics.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
public:
    	MyPhysicsList();
    	~MyPhysicsList();
};


#endif             // always include this at the end 
