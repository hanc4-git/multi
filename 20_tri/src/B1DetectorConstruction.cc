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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Detector parameters
  //
  G4double detector_sizeX = 20*mm, detector_sizeY = 20*mm, detector_sizeZ = .5*mm;
  G4double detector_offset_z = 30*mm;

  // define Elements
  G4Element* elCs = new G4Element("Caesium", "Cs", 55., 132.905*g/mole);
  G4Element* elPb = new G4Element("Lead"   , "Pb", 82., 207.20*g/mole);
  G4Element* elBr = new G4Element("Bromine", "Br", 35., 79.904*g/mole);

  // define simple materials
  // define a material from elements.   case: chemical molecule
  G4Material* detector_mat = new G4Material("Semiconductor", 4.42*g/cm3, 3);
  detector_mat->AddElement(elCs, 1);
  detector_mat->AddElement(elPb, 1);
  detector_mat->AddElement(elBr, 3);

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeX = 10000*detector_sizeX;
  G4double world_sizeY = 10000*detector_sizeY;
  G4double world_sizeZ = 10000*detector_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",				//its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);	//its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,		//its solid
                        world_mat,		//its material
                        "World");		//its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,			//no rotation
                      G4ThreeVector(),		//at (0,0,0)
                      logicWorld,		//its logical volume
                      "World",			//its name
                      0,			//its mother  volume
                      false,			//no boolean operation
                      0,			//copy number
                      checkOverlaps);		//overlaps checking
                     
  //     
  // Detector
  //  
  G4Box* solidDetector =    
    new G4Box("Detector",			//its name
        0.5*detector_sizeX, 0.5*detector_sizeY, 0.5*detector_sizeZ);	//its size
      
  G4LogicalVolume* logicDetector =                         
    new G4LogicalVolume(solidDetector,		//its solid
                        detector_mat,		//its material
                        "Detector");		//its name
               
  new G4PVPlacement(0,				//no rotation
                    G4ThreeVector(0,0,detector_offset_z),	//at (0,0,0)
                    logicDetector,		//its logical volume
                    "Detector",			//its name
                    logicWorld,			//its mother  volume
                    false,			//no boolean operation
                    1,				//copy number
                    checkOverlaps);		//overlaps checking

  //
  // Shape 1
  //
  G4ThreeVector pos1 = G4ThreeVector(0, 0, 31*mm);

  // Detector shape 1
  G4double shape1_sizeX = 20*mm, shape1_sizeY = 20*mm, shape1_sizeZ = .5*mm;
  G4Box* solidShape1 =
    new G4Box("Shape1",				//its name
	0.5*shape1_sizeX, 0.5*shape1_sizeY, 0.5*shape1_sizeZ); //its size

  G4LogicalVolume* logicShape1 =
    new G4LogicalVolume(solidShape1,		//its solid
			detector_mat,		//its material
			"Shape1");		//its name

  new G4PVPlacement(0,				//no rotation
		    pos1,			//at position
		    logicShape1,		//its logical volume
		    "Shape1",			//its name
		    logicWorld,			//its mother  volume
		    false,			//no boolean operation
		    1,				//copy number
		    checkOverlaps);		//overlaps checking

  //
  // Shape 2
  //
  G4ThreeVector pos2 = G4ThreeVector(0, 0, 32*mm);

  // Detector shape 2
  G4double shape2_sizeX = 20*mm, shape2_sizeY = 20*mm, shape2_sizeZ = .5*mm;
  G4Box* solidShape2 =
    new G4Box("Shape2",				//its name
	0.5*shape2_sizeX, 0.5*shape2_sizeY, 0.5*shape2_sizeZ); //its size

  G4LogicalVolume* logicShape2 =
    new G4LogicalVolume(solidShape2,		//its solid
			detector_mat,		//its material
			"Shape2");		//its name

  new G4PVPlacement(0,				//no rotation
		    pos2,			//at position
		    logicShape2,		//its logical volume
		    "Shape2",			//its name
		    logicWorld,			//its mother  volume
		    false,			//no boolean operation
		    1,				//copy number
		    checkOverlaps);		//overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
