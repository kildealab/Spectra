#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AccumulableManager.hh"

// For randomizing the seed
#include "Randomize.hh"
#include <time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() //(HistoManager* histo)
 : G4UserRunAction(),
//    fHistoManager(histo),
    fp1Dose(0.),
    fp2Dose(0.),
    fp3Dose(0.),
    fC1Dose(0.),
    fC2Dose(0.),
    fC3Dose(0.),
    fN1Dose(0.),
    fN2Dose(0.),
    fN3Dose(0.),
    fO1Dose(0.),
    fO2Dose(0.),
    fO3Dose(0.),
    fa1Dose(0.),
    fa2Dose(0.),
    fa3Dose(0.),
    fd1Dose(0.),
    fd2Dose(0.),
    fd3Dose(0.),
    fe1Dose(0.),
    fe2Dose(0.),
    fe3Dose(0.),
    fep1Dose(0.),
    fep2Dose(0.),
    fep3Dose(0.)
{
    // Set the seed according to the current system time
    G4long seed=time(0);
    G4Random::setTheSeed(seed);

    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetVerboseLevel(0);

//    if ( IsMaster() ) {
//
    // Define histograms start values
    const G4int kMaxHisto = 27;
    const G4String id[] = { "0", "1", "2", "3" , "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
                          "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"};
    const G4String title[] =
             { "energy spectrum of protons in scorer 1",               // 0
               "energy spectrum of carbon nuclei in scorer 1",         // 1
               "energy spectrum of nitrogen nuclei in scorer 1",       // 2
               "energy spectrum of oxygen nuclei in scorer 1",         // 3
               "energy spectrum of alphas in scorer 1",                // 4
               "energy spectrum of deuterons in scorer 1",             // 5
               "energy spectrum of electrons in scorer 1",             // 6
               "energy spectrum of positrons in scorer 1",             // 7
               "energy spectrum of gammas in scorer 1",                // 8
               "energy spectrum of protons in scorer 2",               // 9
               "energy spectrum of carbon nuclei in scorer 2",         // 10
               "energy spectrum of nitrogen nuclei in scorer 2",       // 11
               "energy spectrum of oxygen nuclei in scorer 2",         // 12
               "energy spectrum of alphas in scorer 2",                // 13
               "energy spectrum of deuterons in scorer 2",             // 14
               "energy spectrum of electrons in scorer 2",             // 15
               "energy spectrum of positrons in scorer 2",             // 16
               "energy spectrum of gammas in scorer 2",                // 17
               "energy spectrum of protons in scorer 3",               // 18
               "energy spectrum of carbon nuclei in scorer 3",         // 19
               "energy spectrum of nitrogen nuclei in scorer 3",       // 20
               "energy spectrum of oxygen nuclei in scorer 3",         // 21
               "energy spectrum of alphas in scorer 3",                // 22
               "energy spectrum of deuterons in scorer 3",             // 23
               "energy spectrum of electrons in scorer 3",             // 24
               "energy spectrum of positrons in scorer 3",             // 25
               "energy spectrum of gammas in scorer 3"                 // 26
              };
              
    // Default values (can be reset via /analysis/h1/set command)               
    G4int nbins = 500;
    G4double vmin = 1.;
    G4double vmax = 10000000.;
    const G4String& unitName = "none";
    const G4String& fcnName = "none";
    const G4String& binScheme = "log";
  
    // Create all histograms as activated
    for (G4int k=0; k<kMaxHisto; k++) {
      G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax, unitName, fcnName, binScheme);
      analysisManager->SetH1Activation(ih, true);
    }

    analysisManager->CreateNtuple(
            "RelDose",
            "Relative dose contributions by particle"
            );

    analysisManager->CreateNtupleDColumn("p,1");
    analysisManager->CreateNtupleDColumn("p,2");
    analysisManager->CreateNtupleDColumn("p,3");
                                               
    analysisManager->CreateNtupleDColumn("C,1");
    analysisManager->CreateNtupleDColumn("C,2");
    analysisManager->CreateNtupleDColumn("C,3");
                                               
    analysisManager->CreateNtupleDColumn("N,1");
    analysisManager->CreateNtupleDColumn("N,2");
    analysisManager->CreateNtupleDColumn("N,3");
                                               
    analysisManager->CreateNtupleDColumn("O,1");
    analysisManager->CreateNtupleDColumn("O,2");
    analysisManager->CreateNtupleDColumn("O,3");
                                               
    analysisManager->CreateNtupleDColumn("a,1");
    analysisManager->CreateNtupleDColumn("a,2");
    analysisManager->CreateNtupleDColumn("a,3");
                                               
    analysisManager->CreateNtupleDColumn("d,1");
    analysisManager->CreateNtupleDColumn("d,2");
    analysisManager->CreateNtupleDColumn("d,3");
                                               
    analysisManager->CreateNtupleDColumn("e,1");
    analysisManager->CreateNtupleDColumn("e,2");
    analysisManager->CreateNtupleDColumn("e,3");

    analysisManager->CreateNtupleDColumn("ep,1");
    analysisManager->CreateNtupleDColumn("ep,2");
    analysisManager->CreateNtupleDColumn("ep,3");

    analysisManager->FinishNtuple();


    G4AccumulableManager* accMan = G4AccumulableManager::Instance();

    // Relative dose accumulables
    // protons
    accMan->RegisterAccumulable(fp1Dose);
    accMan->RegisterAccumulable(fp2Dose);
    accMan->RegisterAccumulable(fp3Dose);
    // carbon nuclei
    accMan->RegisterAccumulable(fC1Dose);
    accMan->RegisterAccumulable(fC2Dose);
    accMan->RegisterAccumulable(fC3Dose);
    // nitrogen nuclei
    accMan->RegisterAccumulable(fN1Dose);
    accMan->RegisterAccumulable(fN2Dose);
    accMan->RegisterAccumulable(fN3Dose);
    // oxygen nuclei
    accMan->RegisterAccumulable(fO1Dose);
    accMan->RegisterAccumulable(fO2Dose);
    accMan->RegisterAccumulable(fO3Dose);
    // alphas
    accMan->RegisterAccumulable(fa1Dose);
    accMan->RegisterAccumulable(fa2Dose);
    accMan->RegisterAccumulable(fa3Dose);
    // deuterons
    accMan->RegisterAccumulable(fd1Dose);
    accMan->RegisterAccumulable(fd2Dose);
    accMan->RegisterAccumulable(fd3Dose);
    // electrons
    accMan->RegisterAccumulable(fe1Dose);
    accMan->RegisterAccumulable(fe2Dose);
    accMan->RegisterAccumulable(fe3Dose);
    // positrons
    accMan->RegisterAccumulable(fep1Dose);
    accMan->RegisterAccumulable(fep2Dose);
    accMan->RegisterAccumulable(fep3Dose);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
//    delete fHistoManager;
    delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4AccumulableManager* accMan = G4AccumulableManager::Instance();
    accMan->Reset();


    //if ( IsMaster() ) {

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();

    //}

//    fHistoManager->Book();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{

    G4AccumulableManager* accMan = G4AccumulableManager::Instance();
    accMan->Merge();

//    if ( IsMaster() ) {

    G4double total1 = fp1Dose.GetValue() + fC1Dose.GetValue() + fN1Dose.GetValue()
        + fO1Dose.GetValue() + fa1Dose.GetValue() + fd1Dose.GetValue() + fe1Dose.GetValue()
        + fep1Dose.GetValue();
    
    G4double total2 = fp2Dose.GetValue() + fC2Dose.GetValue() + fN2Dose.GetValue()
        + fO2Dose.GetValue() + fa2Dose.GetValue() + fd2Dose.GetValue() + fe2Dose.GetValue()
        + fep2Dose.GetValue();
    
    G4double total3 = fp3Dose.GetValue() + fC3Dose.GetValue() + fN3Dose.GetValue()
        + fO3Dose.GetValue() + fa3Dose.GetValue() + fd3Dose.GetValue() + fe3Dose.GetValue()
        + fep3Dose.GetValue();
    
//    fHistoManager->FillNTuple(
//        fp1Dose.GetValue() / total1, fp2Dose.GetValue() / total2, fp3Dose.GetValue() / total3,
//        fC1Dose.GetValue() / total1, fC2Dose.GetValue() / total2, fC3Dose.GetValue() / total3,
//        fN1Dose.GetValue() / total1, fN2Dose.GetValue() / total2, fN3Dose.GetValue() / total3,
//        fO1Dose.GetValue() / total1, fO2Dose.GetValue() / total2, fO3Dose.GetValue() / total3,
//        fa1Dose.GetValue() / total1, fa2Dose.GetValue() / total2, fa3Dose.GetValue() / total3,
//        fd1Dose.GetValue() / total1, fd2Dose.GetValue() / total2, fd3Dose.GetValue() / total3,
//        fe1Dose.GetValue() / total1, fe2Dose.GetValue() / total2, fe3Dose.GetValue() / total3,
//        fep1Dose.GetValue() / total1, fep2Dose.GetValue() / total2, fep3Dose.GetValue() / total3
//        );
    
    auto analysisManager = G4AnalysisManager::Instance();  
    
    //G4cout << "p, 3: " << fp3Dose.GetValue()/MeV << " MeV" << G4endl;
    //G4cout << "total, 3: " << total1/MeV << " MeV" << G4endl;
    //G4cout << "p/total, 3: " << fp3Dose.GetValue() / total1 << G4endl;
    analysisManager->FillNtupleDColumn(0, fp1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(1, fp2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(2, fp3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(3, fC1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(4, fC2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(5, fC3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(6, fN1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(7, fN2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(8, fN3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(9, fO1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(10, fO2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(11, fO3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(12, fa1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(13, fa2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(14, fa3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(15, fd1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(16, fd2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(17, fd3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(18, fe1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(19, fe2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(20, fe3Dose.GetValue() / total3);
    
    analysisManager->FillNtupleDColumn(21, fep1Dose.GetValue() / total1);
    analysisManager->FillNtupleDColumn(22, fep2Dose.GetValue() / total2);
    analysisManager->FillNtupleDColumn(23, fep3Dose.GetValue() / total3);
    
    // Add in total dose column

    analysisManager->AddNtupleRow();
    
    analysisManager->Write();
    analysisManager->CloseFile();


//    }
    
//    fHistoManager->Save();


}
