//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the B4c::DetectorConstruction class

#include "DetectorConstruction.hh"


namespace B4c
{

G4ThreadLocal


DetectorConstruction::DetectorConstruction()
{
    nist = G4NistManager::Instance();
}


DetectorConstruction::~DetectorConstruction()
{}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();

  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials()
{

  // -----------------Materials-----------------
    
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    water = nist->FindOrBuildMaterial("G4_WATER");
    Aluminum = nist->FindOrBuildMaterial("G4_Al");
    Glass = nist->FindOrBuildMaterial("G4_GLASS_LEAD");

    CdS = new G4Material("CdS", 4.82*g/cm3, 2);                 //add the density of CdS
    CdS->AddMaterial(nist->FindOrBuildMaterial("G4_Cd"), 1);
    CdS->AddMaterial(nist->FindOrBuildMaterial("G4_S"), 1);

    Scint = new G4Material("Scint", 1.000*g/cm3, 2);            //add the density of scintillator
    Scint->AddMaterial(water, 90.0*perCent);
    Scint->AddMaterial(CdS, 10.0*perCent);

    
   // ------------ Generate & Add Material Properties Table ------------
        
//Scintillator:
        // working for Cherenkov 14/3/22
        G4double energy[2] = {2.034*eV, 4.136*eV};      //energy of photons
        G4double rindexScint [2] = {1.34, 1.36};        //refractive index of scint
        G4double rindexWorld [2] = {1.00, 1.00};        //refractive index of world
    
//        std::vector<G4double> energy = {2.034*eV, 3.021*eV, 4.136*eV};     //energy of photons
//        std::vector<G4double> rindexScint = {1.34, 1.35, 1.36};            //refractive index of scint
//        std::vector<G4double> rindexWorld = {1.00, 1.00, 1.00};            //refractive index of world
    
        //for Scintillation 15/3/22
        G4double absorption[32] = {3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m,  12.346 * m, 13.889 * m,
        15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
        45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
        52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
        30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
        17.500 * m, 14.500 * m};
    
        G4double scintFast[32] = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
        1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
    
        G4double scintSlow[32] = {0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
        7.00, 6.00, 4.00, 3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00, 4.00,
        5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00};
    
        mptScint = new G4MaterialPropertiesTable();
     //dependant on energy
        mptScint->AddProperty("RINDEX", energy, rindexScint, 2);
        mptScint->AddProperty("ABSLENGTH", energy, absorption, 32);
        mptScint->AddProperty("SCINTILLATIONCOMPONENT1", energy, scintFast, 32);
        mptScint->AddProperty("SCINTILLATIONCOMPONENT2", energy, scintSlow, 32);
     //independant on energy
        mptScint->AddConstProperty("SCINTILLATIONYIELD", 1357./MeV); //from paper 1357./MeV//
        mptScint->AddConstProperty("RESOLUTIONSCALE", 1.0);
        mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 1.*ns);       //fast time constant of scintillator
        mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10.*ns);      //slow time constant of scintillator
        mptScint->AddConstProperty("SCINTILLATIONYIELD1", 0.8);
        mptScint->AddConstProperty("SCINTILLATIONYIELD2", 0.8);
    
        Scint->SetMaterialPropertiesTable(mptScint);
    
        // Set the Birks Constant for the scintillator (assumption)
        Scint->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
    
//World(Air)
        mptWorld = new G4MaterialPropertiesTable();       //Air (worldMat) properties table
        mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
        worldMat->SetMaterialPropertiesTable(mptWorld);
    
  
    
    // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

}


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  fNofLayers = 10;
    
    // QD Experiment setup:
    
        //-------------World-------------------

        G4Box *solidWorld = new G4Box("solidWorld", 1.*m, 1.*m, 1.*m);

        G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

        G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
    
        //-------------Black Box-------------------

        G4Box *solidBox = new G4Box("solidBox", 0.425*m, 0.27*m, 0.28*m);

        G4LogicalVolume *logicBox = new G4LogicalVolume(solidWorld, worldMat, "logicBox");

        G4VPhysicalVolume *physBox = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBox, "physBox", logicWorld, false, 0, true);
    
        //----------------Stand----------------------
    
        G4Box *solidStand = new G4Box("Stand", 0.06*m, 0.06*m, 0.07325*m);     //standard Geant4 unit is mm, so must use *m to convert to meters
        
        G4LogicalVolume *logicStand = new G4LogicalVolume(solidStand, Aluminum, "Stand");
        
        new G4PVPlacement(0, G4ThreeVector(-0.30*m, 0.*m, -0.205*m), logicStand, "Stand", logicBox, false, 0, true);
        
        //-----------Glass Bottle------------------------
        
        G4Tubs *solidBottle1 = new G4Tubs("Bottle1", 0.*m, 0.04*m, 0.095*m, 0., 2.0*CLHEP::pi);
        
//        G4LogicalVolume *logicBottle1 = new G4LogicalVolume(solidBottle1, Glass, "Bottle1");
//
//        new G4PVPlacement(0, G4ThreeVector(-0.30*m, 0.*m, -0.036*m), logicBottle1, "Bottle1", logicWorld, false, 0, true);
    
    
        G4Tubs *solidBottle2 = new G4Tubs("Bottle2", 0.*m, 0.037*m, 0.087*m, 0., 2.0*CLHEP::pi);
    
//        G4LogicalVolume *logicBottle2 = new G4LogicalVolume(solidBottle2, Glass, "Bottle2");
//
//        new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicBottle2, "Bottle2", logicBottle, false, 0, true);
    
    
        G4SubtractionSolid *bottle = new G4SubtractionSolid ("Bottle", solidBottle1, solidBottle2);
        G4LogicalVolume *logicBottle = new G4LogicalVolume(bottle, Glass, "Bottle");
        new G4PVPlacement(0, G4ThreeVector(-0.30*m, 0.*m, -0.036*m), logicBottle, "Bottle", logicBox, false, 0, true);
    
        //---------------QD---------------------
    
        G4Tubs *solidQD = new G4Tubs("QD", 0.*m, 0.037*m, 0.0335*m, 0., 2.0*CLHEP::pi);

        G4LogicalVolume *logicQD = new G4LogicalVolume(solidQD, Scint, "QD");

        new G4PVPlacement(0, G4ThreeVector(0*m, 0*m, -0.0545*m), logicQD, "QD", logicBottle, false, 0, true);
    
        //---------------PMT---------------------
    
        G4Sphere *absorberS = new G4Sphere("Abso", 0.07*m, 0.076*m, 0.*CLHEP::pi, 1.*CLHEP::pi, 0.*CLHEP::pi, 1.*CLHEP::pi);
    
        G4LogicalVolume *absorberLV = new G4LogicalVolume(absorberS, Glass, "AbsoLV");
    
        G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
        rotationMatrix->rotateZ(-0.5*CLHEP::pi);
    
        new G4PVPlacement(rotationMatrix, G4ThreeVector(-0.15*m, 0.*m, -0.09*m), absorberLV, "Abso", logicBox, false, 0, true);

   
    
    
    
   // G4Scintillation :: G4Scintillation
    
//    G4OpticalSurface* photocath_opsurf=
//       new G4OpticalSurface("photocath_opsurf",glisur,polished,
//                             dielectric_metal);
//    G4double photocath_EFF[num]={1.,1.};
//    G4double photocath_REFL[num]={0.,0.};
//    G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
//    photocath_mt->AddProperty("EFFICIENCY",Ephoton,photocath_EFF,num);
//    photocath_mt->AddProperty("REFLECTIVITY",Ephoton,photocath_REFL,num);
//    photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);
//    new G4LogicalSkinSurface("photocath_surf",photocath_log,photocath_opsurf);

    

    
    
    //Optical surfaces ??
      
    //  G4OpticalSurface* PMT_opsurf= new G4OpticalSurface("PMT_opsurf", glisur, polished, dielectric_metal);

    
    
    

//-----EXTRA Testing------
    
//       G4double innerRadius = 0.*cm;
//       G4double outerRadius = 2.*cm;
//       G4double hz = 3.*cm;                 //half length in Z (so height is 6cm)
//       G4double startingAngle = 0.*deg;
//       G4double spanningAngle = 360.*deg;
//
//       //Outer Cylinder
//       G4ThreeVector pos3 = G4ThreeVector(0*cm, -3*cm, -6*cm);
//       G4Tubs* solidBottle1 = new G4Tubs ("Bottle1", innerRadius, outerRadius, hz, startingAngle, spanningAngle);
//       G4LogicalVolume *logicBottle1 = new G4LogicalVolume (solidBottle1, defaultMaterial, "Bottle1");
//       new G4PVPlacement(0,                       //no rotation
//                         pos3,                    //at position
//                         logicBottle1,             //its logical volume
//                         "Bottle1",                //its name
//                         logicWorld,                //its mother  volume
//                         false,                   //no boolean operation
//                         0,                       //copy number
//                         true);                   //overlaps checking
//
//        //Inner Cylinder
//        G4ThreeVector pos4 = G4ThreeVector(0*cm, 0*cm, 0*cm);
//        G4Tubs* solidBottle2 = new G4Tubs ("Bottle2", 0.*cm, 1.7*cm, 2.7*cm, 0.*deg, 360.*deg);
//        G4LogicalVolume *logicBottle2 = new G4LogicalVolume (solidBottle2, defaultMaterial, "Bottle2");
//        new G4PVPlacement(0,                     //no rotation
//                        pos4,                    //at position
//                        logicBottle2,            //its logical volume
//                        "Bottle2",               //its name
//                        logicBottle1,            //its mother  volume
//                        false,                   //no boolean operation
//                        0,                       //copy number
//                        true);                   //overlaps checking
//
//
//    G4ThreeVector pos = G4ThreeVector(6*cm, 0*cm, 5*cm);
//    G4Torus* solidSample = new G4Torus ("sample", 0.*m, 0.01*m, 0.02*m, 0.*deg, 360.*deg);
//    G4LogicalVolume *logicSample = new G4LogicalVolume (solidSample, worldMat, "sample");
//    new G4PVPlacement (0, pos, logicSample, "sample", logicWorld, false, 0, true);

    


 
// Visualization attributes:

    //-----Black Box-------
  //  logicBox->SetVisAttributes (G4VisAttributes::GetInvisible());

    //-----QD-------
    G4VisAttributes *simpleBox= new G4VisAttributes(G4Colour(0.8,0.8,0.,0.3));
    simpleBox -> G4VisAttributes::SetForceSolid(true);
    //simpleBox->SetVisibility(true);
    logicQD->SetVisAttributes(simpleBox);
    
    //-----Bottle-------
    G4VisAttributes *vissBottle = new G4VisAttributes(G4Colour(0.0,0.0,0.6,0.4));
    vissBottle -> G4VisAttributes::SetForceSolid(true);
    logicBottle->SetVisAttributes(vissBottle);

    //-----Stand-------
    G4VisAttributes *vissStand = new G4VisAttributes(G4Colour(0.9,0.9,0.9,1.0));
    vissStand -> G4VisAttributes::SetForceSolid(true);
    logicStand->SetVisAttributes(vissStand);
    
    
    return physWorld;
    
}



void DetectorConstruction::ConstructSDandField()
{
   //Sensitive detectors
//Abso is the PMT
  auto absoSD = new CalorimeterSD("AbsorberSD", "AbsorberHitsCollection", 1);
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  SetSensitiveDetector("AbsoLV",absoSD);

 //Gap is the QD
  auto gapSD
    = new CalorimeterSD("GapSD", "GapHitsCollection", 1);
  G4SDManager::GetSDMpointer()->AddNewDetector(gapSD);
  SetSensitiveDetector("QD",gapSD);


    
}


}
