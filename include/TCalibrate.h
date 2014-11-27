#ifndef TCALIBRATION_H
#define TCALIBRATION_H

// The user works directly with this class, which takes care of managing all the other classes 
    // Do we want this class to do both Delta and Pad calibrations?
class TCalibration : TNamed {

  public:
    TCalibration();
    virtual ~TCalibration();

    virtual void Print(Option_t *opt = "");    
    virtual void Clear(Option_t *opt = "");    

    void DeltaCal();
    void PadCal();
    
  private:
    TCalibration(Bool_t);
    TCalibration *fCalibration;
    
    void InitCalibration();
    void OpenCalibration();
    void FinishCalibration();

  public:
    // I would love for this to be able to open a GUI that shows sharc statuses
    void InspectDeltaCal();
    void InspectPadCal();

  private:
    // Do we want ..?
    // Do we also want a list of detectors/frontstrips to specify what to calibrate?
    // Do we also want calibration status?
    Bool_t fDeltaSrc ;
    Bool_t fDeltaRun ;
    Bool_t fPadSrc   ;
    Bool_t fPadRun   ;

  ClassDef(TCalibrate,0);
};

#endif
