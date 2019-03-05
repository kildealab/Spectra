#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
//#include "HistoManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{

    //HistoManager* histo = new HistoManager();

    RunAction* runAction = new RunAction(); //(histo);
    SetUserAction(runAction);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{

    //HistoManager* histo = new HistoManager();
    PrimaryGeneratorAction* primGenAction = new PrimaryGeneratorAction();
    RunAction*       runAction            = new RunAction(); //(histo);
    EventAction*     evAction             = new EventAction(runAction); //, histo);
    StackingAction*         stackAction   = new StackingAction();
    SteppingAction*         stepAction    = new SteppingAction(runAction);

    SetUserAction(primGenAction);
    SetUserAction(runAction);
    SetUserAction(evAction);
    SetUserAction(stackAction);
    SetUserAction(stepAction);

}
