#ifndef TSHARCCALIBRATE_H
#define TSHARCCALIBRATE_H

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
    
  private:
    static TCalibrate *fCalibrate;
    TCalibrate(Bool_t);
    
    Bool_t InitDeltaCal(const char *ifname);
    const char *Calibrate(Option_t *opt);
    
    Bool_t OpenCalibration(const char *cfname);
    Bool_t CountCalObjects(const char *objname);
    void CreateCalObject(const char *objtype, Option_t *opt, Int_t det=-1, Int_t fs=-1);
//    void CreateCalObjects(const char *objtype, Int_t det_min, Int_t det_max, Int_t fs_min, Int_t fs_max=-1); // back strips not necessary as they are projections
   
   // option flag allows us to process run/src data in same function
    Bool_t MakeChargeMatrices(Option_t *opt);
    Bool_t GetCentroidsFromData(Option_t *opt);
    Bool_t PutCentroidsInMatrix(Option_t *opt);
    Bool_t PutEnergiesInMatrix(Option_t *opt);
    Bool_t ProduceCalGraphs(Option_t *opt);
    Bool_t ProduceMulGraphs(Option_t *opt);

  public:
    // I would love for this to be able to open a GUI that shows sharc statuses
//    void InspectDeltaCal();
//    void InspectPadCal();
  
  private:
    // Do we want ..?
    // Do we also want a list of detectors/frontstrips to specify what to calibrate?

    Bool_t fSharcInputFlag    ;
    Bool_t fRunCalibrationFlag;
    Bool_t fSrcCalibrationFlag;

    Bool_t fRunChgMatsFlag    ;
    Bool_t fRunChgSpecsFlag   ;
    Bool_t fRunChgFitsFlag    ;
    Bool_t fRunCentMatsFlag   ;
    Bool_t fRunCalcMatsFlag   ;
    Bool_t fRunCalGraphsFlag  ;
    Bool_t fRunMulGraphsFlag  ;
    Bool_t fSrcChgMatsFlag    ;
    Bool_t fSrcChgSpecsFlag   ;
    Bool_t fSrcChgFitsFlag    ;
    Bool_t fSrcCentMatsFlag   ;
    Bool_t fSrcCalcMatsFlag   ;
    Bool_t fSrcCalGraphsFlag  ;
    Bool_t fSrcMulGraphsFlag  ;

  ClassDef(TCalibrate,0)
};

#endif
