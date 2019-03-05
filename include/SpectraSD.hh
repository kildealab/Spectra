#ifndef SpectraSD_h
#define SpectraSD_h 1

#include "SpectraHit.hh"

#include "G4VSensitiveDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SpectraSD : public G4VSensitiveDetector
{
  public:
      SpectraSD(G4String name);
      virtual ~SpectraSD();

      virtual void Initialize(G4HCofThisEvent* HCE);
      virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);


  private:
      SpectraHitsCollection* fHitsCollection;
      G4int fSpectraHCID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif