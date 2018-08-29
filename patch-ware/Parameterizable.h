/*
	File: Parameterizable.h
	Purpose: To handle the logic associated with owning and accessing Parameters.
		By providing a singular way of interacting with Parameters finding and 
		accessing them is made much easier over many different Device types.

	Log:
		8/12/18 - N8
			File Created

*/
#ifndef PARAMETERIZABLE_H
#define PARAMETERIZABLE_H

#include "Parameter.h"

#include <string>
#include <map>

class Parameterizable {

public:

	Parameterizable();

	Parameterizable(const Parameterizable &rhs);

	virtual ~Parameterizable();

	bool virtual setParameter(const std::string &tag, const double value);

	bool getParameter(const std::string &tag, Parameter* &param);

	bool addParameter(const std::string &tag);

	bool hasParameter(const std::string &tag) const;

	void updateParameters();
	
	bool parametersReady() const;

	LinkedList<Parameter> getParameters();

protected:

	void copyParameters(const Parameterizable &other);

	std::map<std::string, Parameter> params;
};

#endif // !PARAMETERIZABLE_H
