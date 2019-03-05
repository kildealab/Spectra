#ifndef HadronElasticPhysicsHP_h
#define HadronElasticPhysicsHP_h 1

#include "globals.hh"
#include "G4HadronElasticPhysics.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadronElasticPhysicsHP : public G4HadronElasticPhysics
{
  public: 
    HadronElasticPhysicsHP(G4int ver = 4); 
   ~HadronElasticPhysicsHP();

  public: 
    virtual void ConstructProcess();
        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
