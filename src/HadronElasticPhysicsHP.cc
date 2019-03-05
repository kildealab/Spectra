#include "HadronElasticPhysicsHP.hh"

#include "G4HadronicProcess.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScattering.hh"
#include "G4ParticleHPThermalScatteringData.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadronElasticPhysicsHP::HadronElasticPhysicsHP(G4int ver)
: G4HadronElasticPhysics(ver)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadronElasticPhysicsHP::~HadronElasticPhysicsHP()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadronElasticPhysicsHP::ConstructProcess()
{
  G4HadronElasticPhysics::ConstructProcess();
  GetNeutronModel()->SetMinEnergy(19.5*MeV);

  G4HadronicProcess* process = GetNeutronProcess();
  G4ParticleHPElastic* model1 = new G4ParticleHPElastic();
  process->RegisterMe(model1);
  process->AddDataSet(new G4ParticleHPElasticData());

  
  model1->SetMinEnergy(4*eV);  
  G4ParticleHPThermalScattering* model2 = new G4ParticleHPThermalScattering();
  process->RegisterMe(model2);
  process->AddDataSet(new G4ParticleHPThermalScatteringData());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......