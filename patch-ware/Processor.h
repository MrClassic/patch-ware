
#ifndef PROCESSOR_H
#define PROCESSOR_H

#ifndef NULL
#define NULL 0x0
#endif

#include "InputTypes.h"
#include <vector>

class Processor {
public:

	Processor(); //<--- make private...?

	Processor(const Processor&);

	virtual ~Processor();

	virtual void process() = 0;

	double* inputAddr(const int &input);

	double** outputAddr(const int &output);

	double* addInput();

	bool removeOutput(double* const &remove);

	void addToOutputs(double* const &output);

	double* paramAddr(const int &param);

	bool setOutputChannel(const int &channel, double* const &output);

	void setInputType(const input_type &);

	//clears all internal vectors
	void clear();

	double frameRate = 48000.;

	std::vector<double>& getInputs();

	std::vector<double*>& getOutputs();

	void assumeValidInputs();

	void assumeValidOutputs();

	void setInputValidation(const unsigned int channel, const bool valid);

	void setOutputValidation(const unsigned int channel, const bool valid);

protected:

	double condenseInputs();

	//arrays
	std::vector<double>	
			inputs,		//array of doubles
			params;		//array of doubles
	
	std::vector<double*>
			outputs;	//array of pointers

	std::vector<bool>
			valid_inputs,
			valid_outputs;

	double currentTime; //time tracker

	//signal condensing algorithm
	input_type type;

};

#endif