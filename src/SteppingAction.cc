#include "SteppingAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(RunAction* runAction)  
    : G4UserSteppingAction(),
        fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    // Kill electrons below 1 MeV and, if they deposit energy or are killed within a scoring volume,
    // add their energy to the appropriate histogram
    const G4ParticleDefinition* particleType = aStep->GetTrack()->GetParticleDefinition();
    G4String pName = particleType->GetParticleName();
    if ( pName == "e-" ) {
    
        //G4cout << pName << " pre: " << aStep->GetPreStepPoint()->GetKineticEnergy()/keV << " keV" << G4endl;
        
        // Interested in the energy at the end of the step
        G4double eKin = aStep->GetPostStepPoint()->GetKineticEnergy();
        //G4cout << pName << " post: " << eKin/keV << " keV" << G4endl;
        if ( eKin <= 1.*MeV ) {

            aStep->GetTrack()->SetTrackStatus(fStopAndKill);

           //G4cout << "Killed and TBR if in scoring volume" << G4endl;
           
            // Use the PreStepPoint to get volume information
            G4String volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
            if ( volName == "Target1" ) {
                fRunAction->Sume1Dose(eKin/eV);
                G4AnalysisManager::Instance()->FillH1(9,eKin/eV);
                
                //G4cout << "An electron with energy " << eKin/keV << " keV was killed in Target1" << G4endl;
            }
            else if ( volName == "Target2" ) {
                fRunAction->Sume2Dose(eKin/eV);
                G4AnalysisManager::Instance()->FillH1(21,eKin/eV);
                
                //G4cout << "An electron with energy " << eKin/keV << " keV was killed in Target2" << G4endl;
            }
            else if ( volName == "Target3" ) {
                fRunAction->Sume3Dose(eKin/eV);
                G4AnalysisManager::Instance()->FillH1(33,eKin/eV);
                
                //G4cout << "An electron with energy " << eKin/keV << " keV was killed in Target3" << G4endl;
            }

        }

    }
    //  don't need to worry about annihilation - the energy carried away by gammas is their rest
    //  mass energy, not kinetic energy. their ranges are probably too short to worry about
    //  including them here, but why not - the only reason they're here is to make sure that I can
    //  accurately say their dose contribution is too small to worry about
//    else if ( pName == "e+" ) {
//    
//        //G4cout << pName << " pre: " << aStep->GetPreStepPoint()->GetKineticEnergy()/keV << " keV" << G4endl;
//        
//        // Interested in the energy at the end of the step
//        G4double eKin = aStep->GetPostStepPoint()->GetKineticEnergy();
//        //G4cout << pName << " post: " << eKin/keV << " keV" << G4endl;
//        if ( eKin <= 1.*MeV ) {
//
//            aStep->GetTrack()->SetTrackStatus(fStopAndKill);
//
//           //G4cout << "Killed and TBR if in scoring volume" << G4endl;
//           
//            // Use the PreStepPoint to get volume information
//            G4String volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
//            if ( volName == "Target1" ) {
//                fRunAction->Sumep1Dose(eKin/eV);
//                G4AnalysisManager::Instance()->FillH1(10,eKin/eV);
//                
//                //G4cout << "A positron with energy " << eKin/keV << " keV was killed in Target1" << G4endl;
//            }
//            else if ( volName == "Target2" ) {
//                fRunAction->Sumep2Dose(eKin/eV);
//                G4AnalysisManager::Instance()->FillH1(22,eKin/eV);
//                
//                //G4cout << "A positron with energy " << eKin/keV << " keV was killed in Target2" << G4endl;
//            }
//            else if ( volName == "Target3" ) {
//                fRunAction->Sumep3Dose(eKin/eV);
//                G4AnalysisManager::Instance()->FillH1(34,eKin/eV);
//                
//                //G4cout << "A positron with energy " << eKin/keV << " keV was killed in Target3" << G4endl;
//            }
//
//        }
//
//    }


}
