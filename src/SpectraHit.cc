#include "SpectraHit.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<SpectraHit>* SpectraHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpectraHit::SpectraHit(G4String pName, G4double eKin)
    : G4VHit(), fPname(pName), fEkin(eKin)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpectraHit::Print()
{
    G4cout << fPname << " was created with initial kinetic energy: " << G4BestUnit(fEkin, "Energy") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......