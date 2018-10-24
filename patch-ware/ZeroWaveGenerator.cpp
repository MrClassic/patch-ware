

#include "ZeroWaveGenerator.h"

ZeroWaveGenerator::ZeroWaveGenerator(){ /* Do Nothing */ }

ZeroWaveGenerator::ZeroWaveGenerator(double amplitude){
    params[AMPLITUDE] = amplitude;
}

ZeroWaveGenerator::ZeroWaveGenerator(const ZeroWaveGenerator &orig){
	
}

ZeroWaveGenerator::~ZeroWaveGenerator(){ /* Do Nothing */ }

void ZeroWaveGenerator::updateWaveOffset(){
    //do nothing, wave has no frequency
}

double ZeroWaveGenerator::generate() {

	//output amplitude. Easy peasy
    return params[AMPLITUDE];

}

//EOF