#ifndef TSHARCINPUT_H
#define TSHARCINPUT_H

#include <Rtypes.h>
#include <TNamed.h>
#include <TVector3.h>

#ifndef __CINT__
#include "TNucleus.h"
#include "TKinematics.h"
#include "TSharcAnalysis.h"
#else 
class TNucleus;
class TKinematics;
class TSharcAnalysis;
#endif


// this class is the brains of the whole package. It is the only class containing specific values and settings
class TSharcInput : public TNamed  { 
  public:
    TSharcInput();  // default  constructor.
    static TSharcInput *Get();
    virtual ~TSharcInput();   

    Bool_t InitSharcInput(const char *filename);
    Bool_t InitSharcInput(void);


    Bool_t UpdateInputFile(const char *); // I TAKE A FILE!

    virtual void Print(Option_t *opt = "");
    virtual void Clear(Option_t *opt = "");
  
  private:
    static TSharcInput *fSharcInput;
    TSharcInput(Bool_t);
    
    void PrintInput();

    void trim(std::string *line, const std::string trimchars = " \f\t\n\r\v");
    Bool_t CopyInputFile(const char *);
    Bool_t ParseInputFile(const char *);

    Bool_t CheckInput();
  private:
    
    void SetRunDeltaCalFlag(Bool_t flag)               { fRunDeltaCal = flag        ; }
    void SetSrcDeltaCalFlag(Bool_t flag)               { fSrcDeltaCal = flag        ; }
    
    void SetZ(UInt_t &Z)                                  { fprotons = Z               ; }
    void SetN(UInt_t &N)                                  { fneutrons = N              ; }
    void SetBeamEperU(Double_t &EperU)                    { feperu = EperU             ; }
    void SetPosOffs(Double_t &x,Double_t &y, Double_t &z) { fposoff.SetXYZ(x,y,z)      ; }
    void SetTargetThickness(Double_t &tt)                 { ftargthick = tt            ; }
    void SetTargetMaterial(const char *tmp)               { ftargmat.assign(tmp)       ; }
    void SetRunDataDir(const char *tmp)                   { frundatadir.assign(tmp)    ; }
    void SetSrcDataDir(const char *tmp)                   { fsrcdatadir.assign(tmp)    ; }
    void AddRunData(const char *tmp)                      { frundata.push_back(tmp)    ; }
    void AddSrcData(const char *tmp)                      { fsrcdata.push_back(tmp)    ; }
    void AddRunIon(const char *tmp)                       { fRunIons.push_back(tmp)    ; }
    void AddSrcIon(const char *tmp)                       { fSrcIons.push_back(tmp)    ; }
    void AddSrcEnergy(Double_t &tmp)                      { fSrcEnergies.push_back(tmp); }
    void SetCalFile(const char *tmp)                      { fcalfile.assign(tmp)       ; } 
    
    void SetRunChgSpecFunction(const char *tmp)           { fRunChgSpecFitFunction = tmp  ; }
    void SetRunChgSpecResolution(Double_t &tmp)           { fRunChgSpecResolution = tmp      ; }
    void SetRunChgSpecThreshold(Double_t &tmp)            { fRunChgSpecThreshold  = tmp      ; }
    void SetRunChgSpecFitRange(Double_t chgmin, Double_t chgmax){ fRunChgSpecFitRange_min = chgmin; fRunChgSpecFitRange_max = chgmax; }
    void SetSrcChgSpecFunction(const char *tmp)           { fSrcChgSpecFitFunction = tmp  ; }
    void SetSrcChgSpecFitRange(Double_t chgmin, Double_t chgmax){ fSrcChgSpecFitRange_min = chgmin; fSrcChgSpecFitRange_max = chgmax; }
    void SetSrcChgSpecResolution(Double_t &tmp)           { fSrcChgSpecResolution = tmp      ; }
    void SetSrcChgSpecThreshold(Double_t &tmp)            { fSrcChgSpecThreshold  = tmp      ; }

    void SetFrontChargeMinMax(Double_t chgmin, Double_t chgmax) { fFrontCharge_min = chgmin ; fFrontCharge_max = chgmax ; }
    void SetBackChargeMinMax(Double_t chgmin, Double_t chgmax)  { fBackCharge_min = chgmin  ; fBackCharge_max = chgmax  ; }
    void SetPadChargeMinMax(Double_t chgmin, Double_t chgmax)   { fPadCharge_min = chgmin   ; fPadCharge_max = chgmax   ; }

  public:
    const char *GetInfileName()                           { return fInfileName.c_str() ; }
    const char *GetInfileData()                           { return fInfileData.c_str() ; }
    Bool_t GetRunDeltaCalFlag()                           { return fRunDeltaCal        ; }
    Bool_t GetSrcDeltaCalFlag()                           { return fSrcDeltaCal        ; }

    UInt_t GetZ()                                         { return fprotons            ; }
    UInt_t GetN()                                         { return fneutrons           ; }
    UInt_t GetA()                                         { return fprotons+fneutrons  ; }
    Double_t GetTargetThickness()                         { return ftargthick          ; }
    Double_t GetBeamEperU()                               { return feperu              ; }
    TNucleus * GetBeamNucleus()                           { return fbeam               ; }
    const char *GetBeamSymbol()                           { return fbeam->GetSymbol()  ; }
    TVector3 GetPosOffs()                                 { return fposoff             ; }
    const char *GetTargetMaterial()                       { return ftargmat.c_str()    ; }
    Double_t GetBeamEnergyMidTarget();                 
    TKinematics *GetRunKinematics(const char *ion, Option_t *opt = "");
   
    ///////////////////////////////////////////////////////////////////////////////////////////
    // these getters are much easier to use in a generalised code that uses the same functions
    const char *GetDataDir(Option_t *opt);
    std::vector<std::string> GetData(Option_t *opt);
    std::vector<std::string> GetIons(Option_t *opt);
    Double_t GetChgSpecFitRangeMin(Option_t *opt);   
    Double_t GetChgSpecFitRangeMax(Option_t *opt);   
    Double_t GetChgSpecResolution(Option_t *opt);   
    Double_t GetChgSpecThreshold(Option_t *opt);   
    const char *GetChgSpecFitFunction(Option_t *opt);
    UInt_t GetIonNumber(const char *ion, Option_t *opt);
    std::vector<double> GetEmeas(UInt_t DET, UInt_t FS, UInt_t BS, const char *ion, Option_t *opt);
    ///////////////////////////////////////////////////////////////////////////////////////////
    
    const char *GetRunDataDir()                           { return frundatadir.c_str() ; }
    const char *GetSrcDataDir()                           { return fsrcdatadir.c_str() ; }
    std::vector<std::string> GetRunData()                 { return frundata            ; }
    std::vector<std::string> GetSrcData()                 { return fsrcdata            ; }
    std::vector<std::string> GetRunIons()                 { return fRunIons            ; }
    std::vector<std::string> GetSrcIons()                 { return fSrcIons            ; }
    std::vector<double> GetSrcEnergies()                  { return fSrcEnergies        ; }
    const char *GetCalFile()                              { return fcalfile.c_str()    ; }

    Double_t GetRunChgSpecFitRangeMin()                   { return fRunChgSpecFitRange_min; }
    Double_t GetRunChgSpecFitRangeMax()                   { return fRunChgSpecFitRange_max; }
    Double_t GetRunChgSpecResolution()                    { return fRunChgSpecResolution; }
    Double_t GetRunChgSpecThreshold()                     { return fRunChgSpecThreshold; }
    const char *GetRunChgSpecFitFunction()                { return fRunChgSpecFitFunction.c_str() ; }
    Double_t GetSrcChgSpecFitRangeMin()                   { return fSrcChgSpecFitRange_min; }
    Double_t GetSrcChgSpecFitRangeMax()                   { return fSrcChgSpecFitRange_max; }
    Double_t GetSrcChgSpecResolution()                    { return fSrcChgSpecResolution; }
    Double_t GetSrcChgSpecThreshold()                     { return fSrcChgSpecThreshold; }
    const char *GetSrcChgSpecFitFunction()                { return fSrcChgSpecFitFunction.c_str() ; }

    Double_t GetFrontChargeMin()                          { return fFrontCharge_min    ; }
    Double_t GetFrontChargeMax()                          { return fFrontCharge_max    ; }
    Double_t GetBackChargeMin()                           { return fBackCharge_min     ; }
    Double_t GetBackChargeMax()                           { return fBackCharge_max     ; }
    Double_t GetPadChargeMin()                            { return fPadCharge_min      ; }
    Double_t GetPadChargeMax()                            { return fPadCharge_max      ; }

    const char *MakeOutputName();
/*
    void SetRunChgMat(const char *tmp)   { frunchgmat.assign(tmp); }
    void SetSrcChgMat(const char *tmp)   { fsrcchgmat.assign(tmp); }
    const char *GetRunChgMat()     { return frunchgmat.c_str()   ; }
    const char *GetSrcChgMat()     { return fsrcchgmat.c_str()   ; }
*/
  private:
    std::string fInfileName;
    std::string fInfileData;
    Bool_t fRunDeltaCal;
    Bool_t fSrcDeltaCal;

    UInt_t      fprotons;           // beam proton number
    UInt_t      fneutrons;          // beam neutron number
    Double_t    feperu;             // beam energy per u [MeV]
    TNucleus    *fbeam;
    TVector3    fposoff;            // sharc position offset
    Double_t    ftargthick;         // target thickness
    std::string ftargmat;           // target material
    std::vector<std::string> fRunIons; // formatted name of run ions
    std::vector<std::string> fSrcIons; // formatted name of src ions
    std::vector<double> fSrcEnergies ; // energy of src ions
    
    std::string frundatadir;        // location of run data
    std::string fsrcdatadir;        // location of source data
    std::vector<std::string> frundata;  // vector of strings containing datafile names
    std::vector<std::string> fsrcdata;
    std::string fcalfile;

    // IGNORE THIS RIGHT NOW //////////////////////////////////////////////////////////////////////
    std::string frunchgmat;        // location of run data charge matrix (if available)
    std::string fsrcchgmat;        // location of source data charge matrix (if available)
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // settings to apply when looping over data           ** use same settings for both run and src data **
    Double_t fFrontCharge_min;  // minimum front charge
    Double_t fFrontCharge_max;  // maximum front charge
    Double_t fBackCharge_min;  // "" back charge
    Double_t fBackCharge_max;
    Double_t fPadCharge_min;  // "" pad charge/
    Double_t fPadCharge_max;

  // settings to apply when fitting charge spectra        ** different settings required for run and src **
  // RUN SPECTRA SETTINGS
    Double_t fRunChgSpecFitRange_min;    // range over which to fit
    Double_t fRunChgSpecFitRange_max;    
    std::string fRunChgSpecFitFunction; // type of fit to do
    Double_t fRunChgSpecResolution;         // resolution of peaks... maybe unnecessary
    Double_t fRunChgSpecThreshold;   // for TSpectrum, threshold used for peak search
  // SRC SPECTRA SETTINGS
    Double_t fSrcChgSpecFitRange_min;   
    Double_t fSrcChgSpecFitRange_max;    
    std::string fSrcChgSpecFitFunction; 
    Double_t fSrcChgSpecResolution;          
    Double_t fSrcChgSpecThreshold;   

  ClassDef(TSharcInput,1)

};

#endif

