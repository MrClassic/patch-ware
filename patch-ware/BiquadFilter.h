/* ******************************************************************
 * File:   BiquadFilter.h
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface through Effect
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
	
	// Parameterizable overloaded methods
	bool virtual setParameter(const std::string &tag, const double value);

	bool getParameter(const std::string &tag, Parameter* &param);

	bool addParameter(const std::string &tag);

	bool hasParameter(const std::string &tag) const;

	LinkedList<Parameter> getParameters();

    
private:

    FIRFilter* fir;
    IIRFilter* iir;
    Patch* patch;

	//pointers to onboard filters' parameters
	std::map<std::string, Parameter*> param_ptrs;
};

#endif	/* BIQUADFILTER_H */

//EOF