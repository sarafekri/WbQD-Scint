#ifndef CONSTRUCTION_HH    //how header files are written (watch Tutorial 3 by Physics Matters on YouTube)
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"  //we used this class
#include "G4VPhysicalVolume.hh"            //we used this class
#include "G4LogicalVolume.hh"              //we used this class
#include "G4Box.hh"                        //we used this class
#include "G4Tubs.hh"                       //we used this class
#include "G4PVPlacement.hh"                //we used this class
#include "G4NistManager.hh"                //we used this class
#include "G4SystemOfUnits.hh"              //this header file allows you to use units such as meters (*m)
#include "CLHEP/Units/SystemOfUnits.h"     //
#include "G4Sphere.hh"                     //we used this class
#include "G4RotationMatrix.hh"
#include "globals.hh"

#include "detector.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction // included by Geant4
{

public:
	MyDetectorConstruction();
	~MyDetectorConstruction();
	
	virtual G4VPhysicalVolume *Construct();

private:
	G4LogicalVolume *logicEnv3;
	virtual void ConstructSDandField();

};



#endif
