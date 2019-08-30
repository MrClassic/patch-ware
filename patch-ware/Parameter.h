/* ************************************************************************
 * File:   Parameter.h
 * Author: Nate Gallegos
 *
 * Log
 *      2/27/18
 *      File Created
 ************************************************************************ */

#ifndef PARAMETER_H
#define	PARAMETER_H

#include "Patch.h"
#include"DynamicInputDevice.h"

#ifndef NULL
#define NULL 0x0
#endif

/* *************************************************************************
 * Class: Parameter
 * Purpose: To model a parameter for an effect that is possibly changed
 *          from outside the body of the Effect via a patched signal in.
 ************************************************************************* */
class InputDevice;
class Parameter : public DynamicInputDevice{
    
public:
    Parameter();
    Parameter(double * const param);
    
	void setPtr(double * const ptr);

	double* getPtr() const ;

    bool setParameter(const double param);
    
    void disconnect();
    
    bool isPatched() const;

	bool process();
    
    //double operators
    Parameter& operator=(const double);
    Parameter& operator+=(const double);
    Parameter& operator-=(const double);
    
    double operator+(const double) const;
    double operator-(const double) const;
    double operator*(const double) const;
    double operator/(const double) const;
    
    bool operator==(const double) const;
    bool operator<(const double) const;
    bool operator>(const double) const;
    bool operator<=(const double) const;
    bool operator>=(const double) const;
    
    Parameter& operator=(const bool);
    
    //Parameter operators
    Parameter& operator=(Parameter &);
    Parameter& operator+=(const Parameter &);
    Parameter& operator-=(const Parameter &);
    
    double operator+(const Parameter &) const;
    double operator-(const Parameter &) const;
    double operator*(const Parameter &) const;
    double operator/(const Parameter &) const;
    
    bool operator==(const Parameter &) const;
    bool operator<(const Parameter &) const;
    bool operator>(const Parameter &) const;
    bool operator<=(const Parameter &) const;
    bool operator>=(const Parameter &) const;
    
    operator bool() const;
	operator int() const;
    operator double() const;
    
    bool addInput(Patch * const patch);
    
    bool removeInput(Patch * const patch);
    
    //friend class SineWaveGenerator; shouldnt need this...
    
private:
    double* param;
    bool patched;
};


	double operator+(const double, const Parameter &);
    double operator-(const double, const Parameter &);
    double operator*(const double, const Parameter &);
    double operator/(const double, const Parameter &);
    
#endif	/* PARAMETER_H */

