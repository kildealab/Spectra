// User defined
#include "DetectorConstruction.hh"
#include "SpectraSD.hh"

// Geant4
#include "globals.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// even though production cuts aren't used by default by G4DNA, they can still be added
// also necessary for the use of standard Physics lists
#include "G4ProductionCuts.hh"

// need the following two classes in order to use "G4_WATER" material - uses NIST data
#include "G4NistManager.hh"
#include "G4Material.hh"

// Physical and Logical volume classes always necessary
// Orb class is necessary to make spheres
/* Tubs
A tube or tube segment with curved sides parallel to
the z-axis. The tube has a specified half-length along
the z-axis, about which it is centered, and a given
minimum and maximum radius. A minimum radius of 0
corresponds to filled tube /cylinder. The tube segment is
specified by starting and delta angles for phi, with 0
being the +x axis, PI/2 the +y axis.
A delta angle of 2PI signifies a complete, unsegmented
tube/cylinder.
*/
#include "G4Orb.hh"
//#include "G4Box.hh"
//#include "G4Tubs.hh"
//#include "G4Ellipsoid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
//#include "G4UnionSolid.hh"
//#include "G4RotationMatrix.hh"
#include "G4Region.hh"

#include "G4PVReplica.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"    // physical and energy extent of program
#include "G4VisAttributes.hh" // for visualization

// For sensitive detector definition and assignment of scorers to it
#include "G4SDManager.hh" 
#include "G4VSensitiveDetector.hh"
#include "G4MultiFunctionalDetector.hh"

//#include <math.h>

using namespace std;
using CLHEP::mm;
using CLHEP::degree;
using CLHEP::nanometer;
using CLHEP::micrometer;
using CLHEP::centimeter;
using CLHEP::meter;

  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() :
    G4VUserDetectorConstruction() // initialize
{
}                               

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// don't need destructor to do anything
DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// this method simply returns the result of the ContructDetector method
G4VPhysicalVolume* DetectorConstruction::Construct() // always use pointers
{
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// this method is to define the geometries
G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

    // water is defined from the NIST material database
    G4NistManager* man = G4NistManager::Instance();
    G4Material* vac = man->FindOrBuildMaterial("G4_Galactic");
    G4Material* icru = man->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRU-4");    
    // G4Material* water = man->FindOrBuildMaterial("G4_WATER");
    // 
    // G4_TISSUE_SOFT_ICRU-4          1      74.9
    //      1         0.101
    //      6         0.111
    //      7         0.026
    //      8         0.762
    // "TS_H_of_Water"

/* *************************************************************************************************
* Make G4_TISSUE_SOFT_ICRU-4 with thermal scattering hydrogen
* ********************************************************************************************** */

    G4String name, symbol;
    G4int z;
    G4double a = 1.01*g/mole;
    G4Element* elH_TS = 
    new G4Element(name="TS_H_of_Water", symbol="H", z=1, a);

    // a = 12.01*g/mole;
    // G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);

    // a = 14.01*g/mole;
    // G4Element* elN  = new G4Element(name="Nitrogen",symbol="N" , z=7., a);

    // a = 16.00*g/mole;
    // G4Element* elO = new G4Element(name="Oxygen", symbol="O", z=8, a);

    G4Element* elC = man->FindOrBuildElement("C");
    G4Element* elN = man->FindOrBuildElement("N");
    G4Element* elO = man->FindOrBuildElement("O");


    G4double density = 1.0*g/cm3;
    G4double ncomp = 4;
    G4double fracmass;
    G4Material* icru_TS = new G4Material("ICRU_TS", density, ncomp);
    icru_TS->AddElement(elH_TS, fracmass=0.101);
    icru_TS->AddElement(elC, fracmass=0.111);
    icru_TS->AddElement(elN, fracmass=0.026);
    icru_TS->AddElement(elO, fracmass=0.762);
    // G4double iPot = icru_TS->GetIonisation()->GetMeanExcitationEnergy();
    // G4cout << G4BestUnit(iPot, "Energy") << G4endl;

    // By default, Geant4 calculates the mean excitation energy as 69.6373 eV
    // ICRU recommends I = 74.9 eV
    icru_TS->GetIonisation()->SetMeanExcitationEnergy(74.9*eV);
    // G4double iPot = icru_TS->GetIonisation()->GetMeanExcitationEnergy();
    // G4cout << G4BestUnit(iPot, "Energy") << G4endl;


/* *************************************************************************************************
* World and ICRU33 Sphere
* ********************************************************************************************** */

    G4double worldRadius = 20.*centimeter; // consider needed source dimensions
    G4double icruRadius  = 15.*centimeter;
    
    // define solid volume
    G4Orb* solidWorld = new G4Orb( 
            "World",       // its name
            worldRadius    // its radius
            );

    // define a logical volume using the solid volume
    fWorldLogical =
        new G4LogicalVolume(
                solidWorld,              // its solid
                vac,                     // its material
                "World"                  // its name
                );

    // place the logical volume within the world
    G4VPhysicalVolume* physiWorld = 
        new G4PVPlacement(0,      // no rotation
                G4ThreeVector(),  // at origin
                "World",          // its name 
                fWorldLogical,    // its logical volume
                0,                // its mother volume (the world)
                false,            // no boolean operations
                0                 // copy number - no copies
                );

    // define icru sphere (15 cm radius sphere of tissue-equivalent material)
    G4Orb* solidICRU = new G4Orb(
            "ICRU",
            icruRadius
            );

    fICRULogical =
        new G4LogicalVolume(
            solidICRU,
            icru,
            "ICRU"
            );

    G4VPhysicalVolume* physiICRU =
        new G4PVPlacement(0,
                G4ThreeVector(),
                "ICRU",
                fICRULogical,
                physiWorld,
                false,
                0
                );

/* *************************************************************************************************
* Targets for scoring spectra
* ********************************************************************************************** */

    G4double targetRadius = 1.5*centimeter;
//    G4double target2Radius = 9.*centimeter;
//    G4double pSPhi        = 0;
//    G4double pDPhi        = 2*M_PIl; // 2*pi
//    G4double pSTheta      = 0;
//    G4double pDTheta      = M_PIl;

    fSolidTarget = new G4Orb(
            "Target",
            targetRadius
            );

//    fSolidTarget2 = new G4Sphere(
//            "Target2",
//            target2Radius - 2*target1Radius,
//            target2Radius,
//            pSPhi,
//            pDPhi,
//            pSTheta,
//            pDTheta
//        );
//
//    fSolidTarget3 = new G4Sphere(
//            "Target3",
//            icruRadius - 2*target1Radius, // inner radius
//            icruRadius,                   // outer radius
//            pSPhi,                        // starting phi angle of the segment (radians)
//            pDPhi,                        // delta phi angle of the segment (radians)
//            pSTheta,                      // starting theta angle of the segment (radians)
//            pDTheta                       // delta theta angle of the segment (radians)
//        );

    fTarget1Logical =
        new G4LogicalVolume(
                fSolidTarget,
                icru,
                "Target1"
                );

    fTarget2Logical =
        new G4LogicalVolume(
            fSolidTarget,
            icru,
            "Target2"
            );

    fTarget3Logical =
        new G4LogicalVolume(
            fSolidTarget,
            icru,
            "Target3"
            );


    G4VPhysicalVolume* physiTarget1 =
        new G4PVPlacement(
                0,
                G4ThreeVector(), // at the centre
                "Target1",
                fTarget1Logical,
                physiICRU,
                false,
                0
                );

    G4VPhysicalVolume* physiTarget2 =
        new G4PVPlacement(
                0,
                G4ThreeVector(0, 0, 7.5*centimeter), // centred at 7.5 cm radial distance from centre
                "Target2",
                fTarget2Logical,
                physiICRU,
                false,
                0
                );

    G4VPhysicalVolume* physiTarget3 =
        new G4PVPlacement(
                0,
                G4ThreeVector(0, 0, 13.5*centimeter), // centred at 13.5 cm radial distance from centre (touching surface of phantom)
                "Target3",
                fTarget3Logical,
                physiICRU,
                false,
                0
                );

//    G4Region* target1Region = 
//        new G4Region("Target1");
//    target1Region->AddRootLogicalVolume(fTarget1Logical);
//
//    G4Region* target2Region = 
//        new G4Region("Target2");
//    target2Region->AddRootLogicalVolume(fTarget2Logical);
//
//    G4Region* target3Region = 
//        new G4Region("Target3");
//    target3Region->AddRootLogicalVolume(fTarget3Logical);


/* *************************************************************************************************
* VISUALIZATION COLOURS
* ********************************************************************************************** */

    G4VisAttributes* visAttributes = 
        new G4VisAttributes(true, G4Colour(1.0, 1.0, 1.0, 0.0));

    fWorldLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes =
        new G4VisAttributes(true, G4Colour(0.0, 0.4, 0.8, 0.3));
    visAttributes->SetForceSolid(true);
    fICRULogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = 
        new G4VisAttributes(true, G4Colour(1.0, 0, 0, 1.0));
    visAttributes->SetForceSolid(true);
    fTarget1Logical->SetVisAttributes(visAttributes);
    //fVisAttributes.push_back(visAttributes);

    //visAttributes =
     //   new G4VisAttributes(true, G4Colour(0, 1.0, 0, 0.5));
    fTarget2Logical->SetVisAttributes(visAttributes);
    //fVisAttributes.push_back(visAttributes);

    //visAttributes =
     //   new G4VisAttributes(true, G4Colour(1.0, 1.0, 1.0, 0.5));
    fTarget3Logical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


    return physiWorld;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer(); 
  SDman->SetVerboseLevel(0);
  G4String SDname;

  G4VSensitiveDetector* spectra1SD = new SpectraSD(SDname="/spectra1");
  SDman->AddNewDetector(spectra1SD);
  fTarget1Logical->SetSensitiveDetector(spectra1SD);

  G4VSensitiveDetector* spectra2SD = new SpectraSD(SDname="/spectra2");
  SDman->AddNewDetector(spectra2SD);
  fTarget2Logical->SetSensitiveDetector(spectra2SD);

  G4VSensitiveDetector* spectra3SD = new SpectraSD(SDname="/spectra3");
  SDman->AddNewDetector(spectra3SD);
  fTarget3Logical->SetSensitiveDetector(spectra3SD);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
