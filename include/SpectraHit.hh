#ifndef SpectraHit_h
#define SpectraHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4AttDef;
class G4AttValue;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SpectraHit : public G4VHit
{
  public:
      SpectraHit(G4String pName, G4double eKin);
      virtual ~SpectraHit() {}

      inline void *operator new(size_t);
      inline void operator  delete(void*aHit);

      void Print();

      void SetName(G4String pName) { fPname = pName; }
      void SetPos(G4double eKin) { fEkin = eKin; }
      G4String GetName() const { return fPname; }
      G4double GetEkin() const { return fEkin; }

      //void SetDose(G4double val) { fDose = val; }
      //G4double GetDose()   const { return fDose; }

  private:
      G4String fPname;
      G4double fEkin;
      //G4double fDose;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Memory allocation stuff to speed things up. /unused/DNAHit.hh has more info.
typedef G4THitsCollection<SpectraHit> SpectraHitsCollection;

extern G4ThreadLocal G4Allocator<SpectraHit>* SpectraHitAllocator;

inline void* SpectraHit::operator new(size_t)
{
    if ( !SpectraHitAllocator )
        SpectraHitAllocator = new G4Allocator<SpectraHit>;
    return (void*)SpectraHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SpectraHit::operator delete(void*aHit)
{
    SpectraHitAllocator->FreeSingle( ( SpectraHit* ) aHit );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif