#ifndef TSHARCCALIBRATE_H
#define TSHARCCALIBRATE_H

#include <Rtypes.h>
#include <TNamed.h>

/*
class TDataManager;
class TFileWriter;
class TFitInfo;
class TFitManager;
class TObjectManager;
class TSharcInput;
class TSharcFormat;
*/
// The user works directly with this class, which takes care of managing all the other classes 
    // Do we want this class to do both Delta and Pad calibrations?
class TCalibrate : public TNamed {

  public:
    virtual ~TCalibrate();
    static TCalibrate *Get();
    virtual void Print(Option_t *opt = "");    
    virtual void Clear(Option_t *opt = "");    

    void DeltaCal(const char *ifname);
//    void PadCal();
    
  private:
    TCalibrate();
    static TCalibrate *fCalibrate;
    
    Bool_t InitDeltaCal(const char *ifname);
    void CreateCalObjects(const char *objtype, Int_t det_min=-1, Int_t det_max=-1, Int_t fs_min=-1, Int_t fs_max=-1); // back strips not necessary as they are projections
//    void OpenCalibration();
    void FinishDeltaCal();

  public:
    // I would love for this to be able to open a GUI that shows sharc statuses
//    void InspectDeltaCal();
//    void InspectPadCal();
    const char *GetInputFile() { return fInputFile; }
  
  private:
    // Do we want ..?
    // Do we also want a list of detectors/frontstrips to specify what to calibrate?
    // Do we also want calibration status?
//    TDataManager   *fDataManager  ;
//    TFileWriter    *fFileWriter   ;
//    TFitInfo       *fFitInfo      ;
//    TFitManager    *fFitManager   ;
//    TObjectManager *fObjectManager;
//    TSharcFormat   *fSharcFormat  ;
//    TSharcInput    *fSharcInput   ;

    const char *fInputFile;      
    Bool_t fRunCal ;
    Bool_t fSrcCal ;

  ClassDef(TCalibrate,0)
};

#endif
