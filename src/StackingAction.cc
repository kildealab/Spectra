#include "StackingAction.hh"
#include "G4StackManager.hh"
//#include "G4DNAChemistryManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction()
    : G4UserStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{

    G4ClassificationOfNewTrack classification = fWaiting;

    const G4ParticleDefinition* particleType = aTrack->GetParticleDefinition();
    G4String pName  = particleType->GetParticleName();
    G4double charge = particleType->GetPDGCharge();
    G4double eKin   = aTrack->GetKineticEnergy();

    // Only looking to kill charged particles and electrons below 1 MeV
    if ( charge != 0.0 ) {

        //if ( (pName != "e-" && pName != "e+") || eKin <= 1.*MeV ) {
        if ( pName != "e-" || eKin <= 1.*MeV ) {

            classification = fKill;
           // G4cout << "Killed: " << pName << ", " << eKin/keV << " keV" << G4endl;

        }
        
//        if ( pName != "proton"  && pName != "deuteron" && pName != "triton" && pName != "alpha"
//                && pName != "Be9" && pName != "Be10" && pName != "B11" && pName != "C12" 
//                && pName != "C13" && pName != "C14" && pName != "C15" && pName != "N14"
//                && pName != "N15" && pName != "O16" && pName != "O17" && pName != "O18" 
//                && pName != "e-" && pName != "e+" ) {
//
//            G4cout << pName << ", " << eKin/keV << " keV killed and not recorded" << G4endl;
//        }

    }

    return classification;

}

/* *************************************************************************************************
 * From the G4UserStackingAction class definition:
 * The NewStage method is called by G4StackManager when the urgentStack becomes empty and contents 
 * in the waitingStack are transferred the urgentStack
 * *************************************************************************************************
 */

void StackingAction::NewStage()
{}
