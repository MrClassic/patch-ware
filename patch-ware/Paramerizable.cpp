
#include "Parameterizable.h"

Parameterizable::Parameterizable() {
	/* Nothing to do...*/
}

Parameterizable::Parameterizable(const Parameterizable &rhs) {
	params = rhs.params;
}

Parameterizable::~Parameterizable() {
	/* Nothing to do */
}

bool Parameterizable::setParameter(const std::string &tag, const double value) {
	if (!hasParameter(tag))
		return false;
	params[tag].setParameter(value);
	return true;
}

bool Parameterizable::addParameter(const std::string &tag) {
	if (hasParameter(tag)) {
		return false;
	}
	params[tag];
	return true;
}

bool Parameterizable::getParameter(const std::string &tag, Parameter* &param) {
	if (!hasParameter(tag))
		return false;
	
	param = &params[tag];
	return true;
}

bool Parameterizable::hasParameter(const std::string &tag) const {
	for (auto it = params.begin(); it != params.end(); ++it) {
		if (it->first == tag)
			return true;
	}
	return false;
}

void Parameterizable::updateParameters() {
	for (auto it = params.begin(); it != params.end(); ++it) {
		it->second.process();
	}
}

bool Parameterizable::parametersReady() const {
	for (auto it = params.begin(); it != params.end(); it++) {
		if (it->second.isPatched() && !it->second.isReady()) {
			//if any parameter is patched and not ready
			//return false
			return false;
		}
	}
	//all parameters are either not patched, 
	//or patched and ready
	return true;
}

LinkedList<Parameter> Parameterizable::getParameters() {
	LinkedList<Parameter> out;
	for (auto it = params.begin(); it != params.end(); ++it) {
		out.push_back(&it->second);
	}
	return out;
}

void Parameterizable::copyParameters(const Parameterizable &other) {
	for (auto it = other.params.begin(); it != other.params.end(); ++it) {
		params[it->first] = (double)it->second;
	}
}