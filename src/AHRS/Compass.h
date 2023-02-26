#pragma once
#include <drivers/HMC5883L.h>
#include "Scheduler/Scheduler.h"
#include "Scheduler/Task.h"

HMC5883L_Simple hmc5883l;

class COMPASS{
public:
    float heading;
    void Init();
    void UpdateCompass();
};

COMPASS compass;

void COMPASS::Init(){
    hmc5883l.SetDeclination(0, 27, 'E');
    hmc5883l.SetSamplingMode(COMPASS_SINGLE);
    hmc5883l.SetScale(COMPASS_SCALE_130);
    hmc5883l.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);
}

void COMPASS::UpdateCompass(){
    heading = hmc5883l.GetHeadingDegrees();
}