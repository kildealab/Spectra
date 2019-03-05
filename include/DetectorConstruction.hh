#ifndef DetectorContruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Orb.hh"
//#include "G4Sphere.hh"

class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    // also define some new members
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    

private:
    G4VPhysicalVolume* ConstructDetector();

    std::vector<G4VisAttributes*> fVisAttributes;

    G4LogicalVolume* fWorldLogical;
    G4LogicalVolume* fICRULogical;
    G4Orb*           fSolidTarget;
    //G4Sphere* fSolidTarget2;
    //G4Sphere* fSolidTarget3;
    G4LogicalVolume* fTarget1Logical;
    G4LogicalVolume* fTarget2Logical;
    G4LogicalVolume* fTarget3Logical;

};
#endif
