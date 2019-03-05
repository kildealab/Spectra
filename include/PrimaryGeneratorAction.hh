#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:

      PrimaryGeneratorAction();
      virtual ~PrimaryGeneratorAction();
      virtual void GeneratePrimaries(G4Event*);

  private:
      G4GeneralParticleSource* fGPS;
};
#endif
