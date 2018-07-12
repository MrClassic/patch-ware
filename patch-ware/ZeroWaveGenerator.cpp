

#include "ZeroWaveGenerator.h"

ZeroWaveGenerator::ZeroWaveGenerator(){
    amplitude = 0.;
    phase = 0.;
    frequency = 1.;
    currentTime = 0;
}

ZeroWaveGenerator::ZeroWaveGenerator(double amplitude){
    this->amplitude = amplitude;
    phase = 0.;
    frequency = 1.;
    currentTime = 0;
}

ZeroWaveGenerator::ZeroWaveGenerator(const ZeroWaveGenerator &orig){
    amplitude = orig.amplitude;
    phase = orig.phase;
    frequency = orig.frequency;
    currentTime = orig.currentTime;
}

ZeroWaveGenerator::~ZeroWaveGenerator(){ }

void ZeroWaveGenerator::updateWaveOffset(){
    //do nothing, wave has no frequency
}

bool ZeroWaveGenerator::pushDouble() {
    if(amplitude.getInputCount() > 0 && !amplitude.isReady()){
        return false;
    }
    output(amplitude);
    return true;
}

