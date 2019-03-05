#ifndef RunAction_h
#define RunAction_h 1

#include "Analysis.hh"
//#include "HistoManager.hh"

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"


class G4Run;
/// Run action class

class RunAction : public G4UserRunAction
{
  public:
      RunAction();
//    RunAction(HistoManager*);
    virtual ~RunAction();

    //virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void Sump1Dose(G4double dose) { fp1Dose += dose; }
    void Sump2Dose(G4double dose) { fp2Dose += dose; }
    void Sump3Dose(G4double dose) { fp3Dose += dose; }
                                                      
    void Sumd1Dose(G4double dose) { fd1Dose += dose; }
    void Sumd2Dose(G4double dose) { fd2Dose += dose; }
    void Sumd3Dose(G4double dose) { fd3Dose += dose; }

    void Sumt1Dose(G4double dose) { ft1Dose += dose; }
    void Sumt2Dose(G4double dose) { ft2Dose += dose; }
    void Sumt3Dose(G4double dose) { ft3Dose += dose; }
                                                      
    void Suma1Dose(G4double dose) { fa1Dose += dose; }
    void Suma2Dose(G4double dose) { fa2Dose += dose; }
    void Suma3Dose(G4double dose) { fa3Dose += dose; }

    void SumBe1Dose(G4double dose) { fBe1Dose += dose; }
    void SumBe2Dose(G4double dose) { fBe2Dose += dose; }
    void SumBe3Dose(G4double dose) { fBe3Dose += dose; }

    void SumB1Dose(G4double dose) { fB1Dose += dose; }
    void SumB2Dose(G4double dose) { fB2Dose += dose; }
    void SumB3Dose(G4double dose) { fB3Dose += dose; }
                                                      
    void SumC1Dose(G4double dose) { fC1Dose += dose; }
    void SumC2Dose(G4double dose) { fC2Dose += dose; }
    void SumC3Dose(G4double dose) { fC3Dose += dose; }
                                                      
    void SumN1Dose(G4double dose) { fN1Dose += dose; }
    void SumN2Dose(G4double dose) { fN2Dose += dose; }
    void SumN3Dose(G4double dose) { fN3Dose += dose; }
                                                      
    void SumO1Dose(G4double dose) { fO1Dose += dose; }
    void SumO2Dose(G4double dose) { fO2Dose += dose; }
    void SumO3Dose(G4double dose) { fO3Dose += dose; }
                                                      
    void Sume1Dose(G4double dose) { fe1Dose += dose; }
    void Sume2Dose(G4double dose) { fe2Dose += dose; }
    void Sume3Dose(G4double dose) { fe3Dose += dose; }

    void Sumep1Dose(G4double dose) { fep1Dose += dose; }
    void Sumep2Dose(G4double dose) { fep2Dose += dose; }
    void Sumep3Dose(G4double dose) { fep3Dose += dose; }


  private:
  	//HistoManager*           fHistoManager;

    G4Accumulable<G4double> fp1Dose;
    G4Accumulable<G4double> fp2Dose;
    G4Accumulable<G4double> fp3Dose;

    G4Accumulable<G4double> fd1Dose;
    G4Accumulable<G4double> fd2Dose;
    G4Accumulable<G4double> fd3Dose;

    G4Accumulable<G4double> ft1Dose;
    G4Accumulable<G4double> ft2Dose;
    G4Accumulable<G4double> ft3Dose;

    G4Accumulable<G4double> fa1Dose;
    G4Accumulable<G4double> fa2Dose;
    G4Accumulable<G4double> fa3Dose;

    G4Accumulable<G4double> fBe1Dose;
    G4Accumulable<G4double> fBe2Dose;
    G4Accumulable<G4double> fBe3Dose;

    G4Accumulable<G4double> fB1Dose;
    G4Accumulable<G4double> fB2Dose;
    G4Accumulable<G4double> fB3Dose;

    G4Accumulable<G4double> fC1Dose;
    G4Accumulable<G4double> fC2Dose;
    G4Accumulable<G4double> fC3Dose;

    G4Accumulable<G4double> fN1Dose;
    G4Accumulable<G4double> fN2Dose;
    G4Accumulable<G4double> fN3Dose;

    G4Accumulable<G4double> fO1Dose;
    G4Accumulable<G4double> fO2Dose;
    G4Accumulable<G4double> fO3Dose;

    G4Accumulable<G4double> fe1Dose;
    G4Accumulable<G4double> fe2Dose;
    G4Accumulable<G4double> fe3Dose;

    G4Accumulable<G4double> fep1Dose;
    G4Accumulable<G4double> fep2Dose;
    G4Accumulable<G4double> fep3Dose;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
