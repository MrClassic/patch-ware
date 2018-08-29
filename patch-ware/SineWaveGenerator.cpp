/* **********************************************************************
 * File:   SineWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      Implemented basic features
 *      6/30/18
 *      Documentation updated
 *		8/16/18
 *		Happy Birthday Wifey!!!
 *		implemented Parameterizable interface
 ********************************************************************* */

#include "SineWaveGenerator.h"

/** ********************************************************************
 * Default Constructor
 * Makes a default sine wave generator. Initializes frequency and amplitude to 
 * the default value of one (1), and the phase and current time to the default
 * value zero (0).
 ********************************************************************* */
SineWaveGenerator::SineWaveGenerator() {
    amplitudeEnvelope = 0.;
    amplitudeEnvelopeStartVal = 0.;
}

/** *********************************************************************
 * General Constructor
 * Accepts the frequency for this sine wave generator as a parameter. The rest 
 * of the fields are decalred as default values.
 * @param frequency - The frequency to initialize this sine wave generator.
 ********************************************************************** */
SineWaveGenerator::SineWaveGenerator(double frequency){
    params["frequency"] = frequency;
    amplitudeEnvelope = 0.;
    amplitudeEnvelopeStartVal = 0.;
}

/** ************************************************************************
 * Copy Constructor
 * Performs a deep copy of the parameterized sine wave generator into the
 * newly constructed object by simply copying over all internal variables.
 * @param orig - an existing sine wave generator to copy.
 ************************************************************************ */
SineWaveGenerator::SineWaveGenerator(const SineWaveGenerator& orig) {
    currentTime = orig.currentTime;
	copyParameters(orig);
	amplitudeEnvelopeStartTime = orig.amplitudeEnvelopeStartTime;
	amplitudeEnvelope = orig.amplitudeEnvelope;
    amplitudeEnvelopeStartVal = orig.amplitudeEnvelopeStartVal;
}

/** *************************************************************************
 * Destructor
 * Does nothing because there is no dynamic memory to clean
 ************************************************************************** */
SineWaveGenerator::~SineWaveGenerator() {
    //do nothing...?
}

//set the frequency and calculate the phase correction to keep the signal 
//continuous.
/* old solution to phase correction problem
     * @depricated
void SineWaveGenerator::setFrequency(double freq){
    
    lastFrequency = (double)frequency;
    frequency.setParameter(freq);
    
    
    double freq = (double)frequency;
    frequency.setParameter(freq);
    
    if(frequency == freq){
        return;
    }
    if((double)frequency != 0){
        double curr = ((currentTime) * freq + getPhaseOffset());
        while ((curr > 1)) {
            curr -= 1;
        }
        while (curr < 0) {
            curr += 1;
        }
        phase.setParameter((double)phase);
        double next = (currentTime * frequency + phase);
        while ((next > 1)) {
            next -= 1;
        }
        while (next < 0) {
            next += 1;
        }
        
        phaseCorrecter = curr - next;
        
        while ((phaseCorrecter > 1)) {
            phaseCorrecter -= 1;
        }
        while (phaseCorrecter < 0) {
            phaseCorrecter += 1;
        }
    }
     * *
}
*/

/** **********************************************************************
 * Update Wave Offset
 * Updates the wave offset for this sine wave generator by consulting the 
 * last frequency, last amplitude and last phase, as well as the current values
 * for frequency, amplitude, and phase.
 * This method updates the private phaseCorrecter variable.
 *********************************************************************** */
void SineWaveGenerator::updateWaveOffset(){
    
    //phaseCorrecter = 
    /*double inSine = 360 * (lastFrequency * currentTime + lastPhase + phaseCorrecter);
    double sin = sineD(inSine);
    double asin = asineD( (lastAmplitude / (double)amplitude) * sin) * 1./360.; 
    double afterAsin = asin - (frequency * currentTime) - (double)phase;
    phaseCorrecter = afterAsin;
    */
    
    phaseCorrecter = lastFrequency * currentTime + lastPhase + phaseCorrecter - (params["frequency"] * currentTime + params["phase"]);
    
}

/** **********************************************************************
 * Push Double
 * Pushes the next double into this sine wave generator's output channel(s).
 * @return false if the sine wave generator is not ready to push its next value.
 *      i.e. if it's inputs are not ready.
 *      true if the double was pushed out successfully.
 *********************************************************************** */
bool SineWaveGenerator::process(){
    
    if(!paramsReady()){
        return false;
    }
    
    //force update on parameters before
    //getting the phase offset
    setFrequency((double)params["frequency"]);
    setAmplitude((double)params["amplitude"]);
    setPhase((double)params["phase"]);
    
    //get phase offset
    double offset = getPhaseOffset();
    
    //send signal to output
	double in = 360.0 * (params["frequency"] * currentTime + offset);
    double out = sineD(in);
    
    //amplitudeEnvelope = amplitudeEnvelopeStartTime
    
    output(params["amplitude"] * out);
    
    return true;
}

//EOF