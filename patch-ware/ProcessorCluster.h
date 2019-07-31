

#ifndef PROCESSOR_CLUSTER_H
#define PROCESSOR_CLUSTER_H

#include "LinkedList.h"
#include "Processor.h"

class ProcessorCluster : public Processor {

public:

	//bring a friend, why not!
	//Circuits are going to be used to generate
	//Processor Clusters for external code.
	friend class Circuit;

	ProcessorCluster() : Processor(){
		//do nothing
	}

	//process inputs, populate outputs
	void process() {
		if (clusterOrder.isEmpty()) {
			return;
		}

		//move inputs to input mappings
		for (int i = 0; i < inputMappings.size() && i < inputs.size(); ++i) {
			*inputMappings[i] = inputs[i];
		}

		//traversal function for cluster order
		auto func = [](Processor* p, void * /* not used */)->bool {
			p->process();
			return true;
		};

		clusterOrder.apply(func, NULL);

		//move output mappings to actual outputs
		for (int o = 0; o < outputMappings.size() && o < outputs.size(); ++o) {
			*outputs[o] = outputMappings[o];
		}

	}

	//maps the input channel to an INTERNAL input array
	//do not pass an EXTERNAL pointer into this function,
	//no check is made. I am assuming you (future me) won't be an asshole
	//and break this delicate system!!!
	bool mapToInput(const int inputChannel, double * const mapping) {
		if (inputChannel >= inputMappings.size() || inputChannel < 0) {
			return false;
		}
		inputMappings[inputChannel] = mapping;
		return true;
	}

	//maps the INTERNAL pointer to an output channel.
	//do not pass EXTERNAL pointers into this function or 
	//you (future me) will break this shit!
	//be kind to "past me" (present me as i type this) 
	//and let me assign this pointer without
	//worrying about validation!!!
	bool mapToOutput(const int outputChannel, double *  &out) {
		if (outputChannel >= outputMappings.size() || outputChannel >= outputs.size()) {
			return false;
		}

		out = &outputMappings[outputChannel];
	}

	LinkedList<Processor> getOrderList() {
		return clusterOrder;
	}

	void setOrderList(LinkedList<Processor> list) {
		clusterOrder = list;
	}

protected:

	LinkedList<Processor> clusterOrder;

	/*
		inputs >> input mappings >> [INTERNAL Processors] >> output mappings >> outputs
	*/
	std::vector<double*> inputMappings;
	std::vector<double> outputMappings;

};

#endif