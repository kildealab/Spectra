#ifndef EventAction_h
#define EventAction_h 1

#include "RunAction.hh"
//#include "HistoManager.hh"

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:

    EventAction(RunAction*); //, HistoManager*);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

private:

    RunAction* fRunAction;
    //HistoManager*     fHistoManager;
    G4int fSpectra1HCID;
    G4int fSpectra2HCID;
    G4int fSpectra3HCID;

};

#endif
