
#include "Parameterizable.h"

Parameterizable::Parameterizable() {
	/* Nothing to do...*/
}

Parameterizable::Parameterizable(const Parameterizable &rhs) {
	//params = rhs.params;
	copyParameters(rhs);
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
	//params[tag]; now done in next line
	paramsList.push_back(&params[tag]);
	return true;
}

bool Parameterizable::getParameter(const std::string &tag, Parameter* &param) {
	if (!hasParameter(tag))
		return false;
	
	param = &params[tag];
	return true;
}

bool Parameterizable::hasParameter(const std::string &tag) const {
	return (params.find(tag) != params.end());
	/* old way
	for (auto it = params.begin(); it != params.end(); ++it) {
		if (it->first == tag)
			return true;
	}
	return false;
	*/
}

void Parameterizable::updateParameters() {
	auto arg = [](Parameter* p, void* /*not used*/)->bool {
		p->process();
		return true;
	};
	paramsList.apply(arg, NULL);
	/* old way (map iterator is slow)
	for (auto it = params.begin(); it != params.end(); ++it) {
		it->second.process();
	}
	*/
}

bool Parameterizable::parametersReady() const {
	auto arg = [](Parameter* p, void* /*not used*/)->bool {
		return !(p->isPatched() && !p->isReady());
	};
	return paramsList.apply(arg, NULL);
	/* old way
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
	*/
}

LinkedList<Parameter> Parameterizable::getParameters() {
	/* old way
	LinkedList<Parameter> out;
	for (auto it = params.begin(); it != params.end(); ++it) {
		out.push_back(&it->second);
	}
	return out;
	*/
	return paramsList; //<-- shallow copy returned
}

void Parameterizable::copyParameters(const Parameterizable &other) {	
	for (auto it = other.params.begin(); it != other.params.end(); ++it) {
		params[it->first] = (double)it->second;
	}
}