#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmLivermorePhysics.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#include "G4PhysicsConstructorRegistry.hh"
#include "QGSP_BIC_HP.hh"

#include "G4NeutronHPManager.hh"

#include "G4EmParameters.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
    G4int verbosity = 0;
    SetVerboseLevel(verbosity);

    // Especially for nuclei.. if the cut isn't set really low than none of the recoil nuclei will even be generated.
    // Their energy will just be said to be deposited within the step that should have generated them as a separate track.
    SetDefaultCutValue(0*mm);
    
    // high-precision elastic scattering of neutrons - valid up to 20 MeV
    RegisterPhysics(new HadronElasticPhysicsHP(verbosity));

    // recommended constructor for high-precision inelastic scattering, capture, and fission in medical applications
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verbosity));

    // WHAT DOES THIS DO
    RegisterPhysics(new G4StoppingPhysics(verbosity));

    // DO I NEED THIS FOR NEUTRONS? DOUBT IT
    RegisterPhysics(new G4IonPhysics(verbosity));

    // WHAT IS THIS FOR
//    RegisterPhysics(new G4IonBinaryCascadePhysics());

    // WHAT DOES THIS DO?
    RegisterPhysics(new G4NeutronTrackingCut(verbosity));

    // WHAT IS THE DIFFERENCE
    RegisterPhysics(new G4DecayPhysics(verbosity));
    RegisterPhysics(new G4RadioactiveDecayPhysics(verbosity));

    // For the photons
    RegisterPhysics(new G4EmLivermorePhysics(verbosity));
   
    //G4NeutronHPManager::GetInstance()->SetVerboseLevel(4); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void PhysicsList::SetCuts()
//{
//
//    SetCutsWithDefault();
//
//    // Kill electrons with kinetic energy less than or equal to 1 MeV if they are produced in the
//    // scoring volumes
//    G4Region*         region;
//    G4String          regName;
//    G4double          eCut = 1.*MeV;
//    G4ProductionCuts* cuts;
//
//    regName = "target1";
//    region = G4RegionStore::GetInstance()->GetRegion(regName);
//    cuts = new G4ProductionCuts;
//    cuts->SetProductionCut(eCut, G4ProductionCuts::GetIndex("e-"));
//    region->SetProductionCuts(cuts);
//
//    regName = "target2";
//    region = G4RegionStore::GetInstance()->GetRegion(regName);
//    cuts = new G4ProductionCuts;
//    cuts->SetProductionCut(eCut, G4ProductionCuts::GetIndex("e-"));
//    region->SetProductionCuts(cuts);
//
//    regName = "target3";
//    region = G4RegionStore::GetInstance()->GetRegion(regName);
//    cuts = new G4ProductionCuts;
//    cuts->SetProductionCut(eCut, G4ProductionCuts::GetIndex("e-"));
//    region->SetProductionCuts(cuts);
//
//}
