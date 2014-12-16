SharcCalibration
================
-> Finish TSharcFormat and replace TSharcName with it                         
-> Get TCalibrate up and running so that compiles                         
-> Get TCalibrate to make charge matrices                         
-> Make and fit charge spectra   
-> Make centroid matrices
-> Make energy calculated matrices
-> Make calibration graphs (run only)     

in order to avoid remaking charge spectra every time we want to try a new set of paramerters we need to be able to update sharc input. Properties will be overwritten with the new file variables, and vectors (so ion names, energies, tree files) WILL NOT BE UPDATED!!!

consider separate input files for run, src chgmats etc...


The private variable fInputData variable should be obtained using the built in PRINT STATEMENT!! (LIKE TCHANNEL) which maybe returns a const char*
make sure Clear is being used
include consistency check on input to prevent garbage settings


WHERE ARE MY CALIBRATIONS???? 
also excitation energies... now. 


