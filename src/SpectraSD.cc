#include "SpectraSD.hh"
#include "SpectraHit.hh"
//#include "HistoManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4VSolid.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpectraSD::SpectraSD(G4String name)
    : G4VSensitiveDetector(name),
      fHitsCollection(0),
      fSpectraHCID(-1)
{
    G4String hcName = "spectraColl";
    collectionName.insert(hcName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpectraSD::~SpectraSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpectraSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection = new SpectraHitsCollection(SensitiveDetectorName, collectionName[0]);

    if ( fSpectraHCID<0 )
    {
        fSpectraHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }
    hce->AddHitsCollection(fSpectraHCID, fHitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SpectraSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // G4double edep = step->GetTotalEnergyDeposit();
    // if ( edep == 0. ) return true;

    // G4cout << "A step happened." << G4endl;

    // Get the number of secondaries produced in the current step
    G4int nSec = step->GetNumberOfSecondariesInCurrentStep();
    if ( nSec == 0. ) return true;

    // G4cout << nSec << " secondaries produced in this region before processing" << G4endl;

    // Get secondaries produced in the current step - not the same as GetSecondary
    // This method starts with GetSecondary, then returns the sub-list that makes up its last n entries,
    //      where n is the number of secondaries produced in that step. It's weird.
    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();

    // Start an instance of the GenericIonsManager to get the recoil nuclei
    

    // Record name and kinetic energy of each of the secondary particles of interest - but only record
    //      if those particles are present
    for (G4int i=0; i<nSec; i++) {
        const G4Track* track = (*secondaries)[i];
        const G4ParticleDefinition* particle = track->GetParticleDefinition();
        G4String pName = particle->GetParticleName();
        G4double eKin = track->GetKineticEnergy();

        // >1 MeV electrons are handled separately
        if ( pName != "e-" || eKin <= 1.*MeV ) {

            SpectraHit* hit = new SpectraHit(pName, eKin);
            fHitsCollection->insert(hit);

        }

        // G4cout << pName << " produced with energy " << G4BestUnit(eKin, "Energy") << " in SpectraSD" << G4endl;


    }

    
    
    return true;
}
