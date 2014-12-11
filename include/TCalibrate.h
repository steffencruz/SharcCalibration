#ifndef TSHARCCALIBRATE_H
#define TSHARCCALIBRATE_H

#include <map>

#include <Rtypes.h>
#include <TNamed.h>

// The user works directly with this class, which takes care of managing all the other classes 
    // Do we want this class to do both Delta and Pad calibrations?
class TCalibrate : public TNamed {

  public:
    TCalibrate();
    virtual ~TCalibrate();
    static TCalibrate *Get();
    
    virtual void Print(Option_t *opt = "");    
    virtual void Clear(Option_t *opt = "");    

  public:
    void DeltaCal(const char *ifname);
    
    void LoadCal(const char *ifname);
    void SaveCal();
    
    Bool_t GetFlag(const char *flagname, Option_t *opt = "");
  private:
    static TCalibrate *fCalibrate;
    TCalibrate(Bool_t);

    Bool_t InitDeltaCal(const char *ifname);
    const char *CalibrateSeparate(Option_t *opt);
    void SetFlag(const char *flagname, Option_t *opt, Bool_t flag);
    
    Bool_t OpenCalibration(const char *cfname);
    
    Bool_t ExistInFile(const char *objname);
    void CreateCalObject(const char *objtype, Option_t *opt, Int_t det=-1, Int_t fs=-1);
   
   // option flag allows us to process run/src data in same function
    Bool_t MakeChargeMatrices(Option_t *opt);
    Bool_t GetCentroidsFromData(Option_t *opt);
    Bool_t PutCentroidsInMatrix(Option_t *opt);
    Bool_t PutEnergiesInMatrix(Option_t *opt);
    Bool_t ProduceCalGraphs(Option_t *opt);
    Bool_t ProduceSrcCalGraphs(Option_t *opt="");
//    Bool_t ProduceMulGraphs(Option_t *opt);
    Bool_t ProduceCombinedCalGraphs(Option_t *opt);

  public:
    // I would love for this to be able to open a GUI that shows sharc statuses
//    void InspectDeltaCal();
//    void InspectPadCal();
  
  private:
    // Do we want ..?
    // Do we also want a list of detectors/frontstrips to specify what to calibrate?
    std::map<std::string,bool> fCalFlags;
  ClassDef(TCalibrate,0)
};

#endif
