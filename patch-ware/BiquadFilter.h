/* ******************************************************************
 * File:   BiquadFilter.h
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/11/17
 *      File Created
 ***************************************************************** */

#include "FIRFilter.h"
#include "IIRFilter.h"

#ifndef BIQUADFILTER_H
#define	BIQUADFILTER_H

class BiquadFilter : public Effect{
public:
    BiquadFilter();
    BiquadFilter(const BiquadFilter& orig);
    ~BiquadFilter();
    bool process();
    
    bool addOutput(Patch * const patch);
    bool addInput(Patch * const patch);
    virtual bool removeOutput(Patch * const patch);
    bool removeInput(Patch * const patch);
    
    void seta0(double coefficient);
    void seta1(double coefficient);
    void seta2(double coefficient);
    void setb0(double coefficient);
    void setb1(double coefficient);
    void setb2(double coefficient);
    
    Parameter& geta0() const;
    Parameter& geta1() const;
    Parameter& geta2() const;
    Parameter& getb0() const;
    Parameter& getb1() const;
    Parameter& getb2() const;
    
private:

    FIRFilter* fir;
    IIRFilter* iir;
    Patch* patch;
};

#endif	/* BIQUADFILTER_H */

//EOF