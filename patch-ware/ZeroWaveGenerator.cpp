

#include "ZeroWaveGenerator.h"

ZeroWaveGenerator::ZeroWaveGenerator(){ /* Do Nothing */ }

ZeroWaveGenerator::ZeroWaveGenerator(double amplitude){
    params["amplitude"] = amplitude;
}

ZeroWaveGenerator::ZeroWaveGenerator(const ZeroWaveGenerator &orig){
	copyParameters(orig);
    currentTime = orig.currentTime;
}

ZeroWaveGenerator::~ZeroWaveGenerator(){ /* Do Nothing */ }

void ZeroWaveGenerator::updateWaveOffset(){
    //do nothing, wave has no frequency
}

bool ZeroWaveGenerator::process() {

	//only check amplitude because its the only parameter that matters
	//for a Zero Wave Generator
    if(params["amplitude"].isPatched() && !params["amplitude"].isReady()){
        return false;
    }

	//output amplitude. Easy peasy
    output(params["amplitude"]);

	//success!
    return true;
}

//EOF