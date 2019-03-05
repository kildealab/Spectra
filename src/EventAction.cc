#include "EventAction.hh"
#include "SpectraHit.hh"
#include "RunAction.hh"
//#include "SteppingAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction) //, HistoManager* histo)
: G4UserEventAction(),
    fRunAction(runAction),
    //fHistoManager(histo),
    fSpectra1HCID(-1),
    fSpectra2HCID(-1),
    fSpectra3HCID(-1)
{
  G4RunManager::GetRunManager()->SetPrintProgress(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
    if ( fSpectra1HCID==-1 )
    {
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        fSpectra1HCID = sdManager->GetCollectionID("spectra1/spectraColl");
    }

    if ( fSpectra2HCID==-1 )
    {
      G4SDManager* sdManager = G4SDManager::GetSDMpointer();
      fSpectra2HCID = sdManager->GetCollectionID("spectra2/spectraColl");
    }

    if ( fSpectra3HCID==-1 )
    {
      G4SDManager* sdManager = G4SDManager::GetSDMpointer();
      fSpectra3HCID = sdManager->GetCollectionID("spectra3/spectraColl");
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get the hits collection of the event
  G4HCofThisEvent* hce = event->GetHCofThisEvent();

  if (!hce)
  {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n";
    G4Exception("EventAction::EndOfEventAction()",
                "Code001",
                JustWarning,
                msg);
    return;
  }

  // Get the spectra collections
  SpectraHitsCollection* spectra1HC = static_cast<SpectraHitsCollection*>(hce->GetHC(fSpectra1HCID));
  SpectraHitsCollection* spectra2HC = static_cast<SpectraHitsCollection*>(hce->GetHC(fSpectra2HCID));
  SpectraHitsCollection* spectra3HC = static_cast<SpectraHitsCollection*>(hce->GetHC(fSpectra3HCID));

  if ( (!spectra1HC) || (!spectra2HC) || (!spectra3HC) )
  {
      G4ExceptionDescription msg;
      msg << "Some of the hits collections of this event not found.\n";
      G4Exception("EventAction::EndOfEventAction()",
              "Code001",
              JustWarning,
              msg);
      return;
  }

  // G4cout << "Scoring region 1 ID: " << &spectra1HC << "\n"
  //        << "Scoring region 2 ID: " << &spectra2HC << "\n"
  //        << "Scoring region 3 ID: " << &spectra3HC << G4endl;

  // Print diagnostics
  //G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  
  //if (printModulo==0 || fEventID % printModulo != 0) return;

  //G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);
  // G4cout << G4endl
  //     << ">>> Event " << fEventID << ">>> Simulation truth : "
  //     << primary->GetG4code()->GetParticleName()
  //     << " " << primary->GetMomentum() << G4endl;

  // Get the number of secondary tracks produced in each sensitive detector
  G4int nSec1 = spectra1HC->entries();
  G4int nSec2 = spectra2HC->entries();
  G4int nSec3 = spectra3HC->entries();

  //G4int nSec = nSec1 + nSec2 + nSec3;
  //G4cout << "There were " << nSec<< " secondaries produced" << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();

  // These loops could also be used to count the number of each particle produced at a given depth using accumulables
  // May need to update SpectraHit definition to take a G4ParticleDefinition rather than a particle name
  G4String pName;
  G4double eKin;
  for (G4int i1=0; i1<nSec1; i1++)
  {
      SpectraHit* particle = (*spectra1HC)[i1];
      //particle->Print();

      pName = particle->GetName();
      eKin = particle->GetEkin();

      //G4cout << pName << " with energy " << eKin/eV << " eV into scorer 1 histo." << G4endl;
      
      //fRunAction->EnergySpectrumOfSecondaries(pName, eKin);

      G4int ih1 = -1; 
      if (pName == "proton") {
          ih1 = 0;
          fRunAction->Sump1Dose(eKin/eV);
      }                             
      else if (pName == "carbon" || pName == "C12" || pName == "C13" || pName == "C14") {
          ih1 = 1;
          fRunAction->SumC1Dose(eKin/eV);
      }   
      else if (pName == "nitrogen" || pName == "N14" || pName == "N14" || pName == "N15") {
          ih1 = 2;
          fRunAction->SumN1Dose(eKin/eV);
      }
      else if (pName == "oxygen" || pName == "O16" || pName == "O17" || pName == "O18") {
          ih1 = 3;
          fRunAction->SumO1Dose(eKin/eV);
      }
      else if (pName == "alpha") {
          ih1 = 4;
          fRunAction->Suma1Dose(eKin/eV);
      }
      else if (pName == "deuteron") {
          ih1 = 5;
          fRunAction->Sumd1Dose(eKin/eV);
      }
      else if (pName == "e-" && eKin <= 1.*MeV) {
          ih1 = 6;
          fRunAction->Sume1Dose(eKin/eV);
      }
      else if (pName == "e+") {
          ih1 = 7;
          fRunAction->Sumep1Dose(eKin/eV);
      }
      else if (pName == "gamma") {
          ih1 = 8;
      }

      //G4cout << "histogram " << ih1 << G4endl;
      if (ih1 > -1) analysisManager->FillH1(ih1,eKin/eV);      
      //if (ih1 > -1) fHistoManager->FillHisto(ih1,eKin);

  }

  for (G4int i2=0; i2<nSec2; i2++)
  {
      SpectraHit* particle = (*spectra2HC)[i2];
      //particle->Print();

      pName = particle->GetName();
      eKin = particle->GetEkin();

      //G4cout << pName << " with energy " << eKin/eV << " eV into scorer 2 histo." << G4endl;
      
      //fRunAction->EnergySpectrumOfSecondaries(pName, eKin);

      G4int ih2 = -1; 
      if (pName == "proton") {
          ih2 = 9;
          fRunAction->Sump2Dose(eKin/eV);
      }
      else if (pName == "carbon" || pName == "C12" || pName == "C13" || pName == "C14") {
          ih2 = 10;
          fRunAction->SumC2Dose(eKin/eV);
      }
      else if (pName == "nitrogen" || pName == "N14" || pName == "N15") {
          ih2 = 11;
          fRunAction->SumN2Dose(eKin/eV);
      }
      else if (pName == "oxygen" || pName == "O16" || pName == "O17" || pName == "O18") {
          ih2 = 12;
          fRunAction->SumO2Dose(eKin/eV);
      }
      else if (pName == "alpha") {
          ih2 = 13;
          fRunAction->Suma2Dose(eKin/eV);
      }
      else if (pName == "deuteron") {
          ih2 = 14;
          fRunAction->Sumd2Dose(eKin/eV);
      }
      else if (pName == "e-" && eKin <= 1.*MeV) {
          ih2 = 15;
          fRunAction->Sume2Dose(eKin/eV);
      }
      else if (pName == "e+") {
          ih2 = 16;
          fRunAction->Sumep2Dose(eKin/eV);
      }
      else if (pName == "gamma") {
          ih2 = 17;
      }

      if (ih2 > -1) analysisManager->FillH1(ih2,eKin/eV);      
      //if (ih2 > -1) fHistoManager->FillHisto(ih2,eKin/eV);      
      
  }

  for (G4int i3=0; i3<nSec3; i3++)
  {
      SpectraHit* particle = (*spectra3HC)[i3];
      //particle->Print();

      pName = particle->GetName();
      eKin = particle->GetEkin();

      //G4cout << pName << " with energy " << eKin/eV << " eV into scorer 3 histo." << G4endl;
      
      //fRunAction->EnergySpectrumOfSecondaries(pName, eKin);

      G4int ih3 = -1; 
      if (pName == "proton") {
          ih3 = 18;
          fRunAction->Sump3Dose(eKin/eV);
      }
      else if (pName == "carbon" || pName == "C12" || pName == "C13" || pName == "C14") {
          ih3 = 19;
          fRunAction->SumC3Dose(eKin/eV);
      }
      else if (pName == "nitrogen" || pName == "N14" || pName == "N15") {
          ih3 = 20;
          fRunAction->SumN3Dose(eKin/eV);
      }
      else if (pName == "oxygen" || pName == "O16" || pName == "O17" || pName == "O18") {
          ih3 = 21;
          fRunAction->SumO3Dose(eKin/eV);
      }
      else if (pName == "alpha") {
          ih3 = 22;
          fRunAction->Suma3Dose(eKin/eV);
      }
      else if (pName == "deuteron") {
          ih3 = 23;
          fRunAction->Sumd3Dose(eKin/eV);
      }
      else if (pName == "e-" && eKin <= 1.*MeV) {
          ih3 = 24;
          fRunAction->Sume3Dose(eKin/eV);
      }
      else if (pName == "e+") {
          ih3 = 25;
          fRunAction->Sumep3Dose(eKin/eV);
      }
      else if (pName == "gamma") {
          ih3 = 26;
      }

      if (ih3 > -1) analysisManager->FillH1(ih3,eKin/eV);      
      //if (ih3 > -1) fHistoManager->FillHisto(ih3,eKin/eV);
      
  }

}
