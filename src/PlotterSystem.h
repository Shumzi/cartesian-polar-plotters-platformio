#pragma once

#include "IMotor.h"
#include "IEncoder.h"
#include "IMode.h"

class PlotterSystem {
private:
    IMotor* motor1;
    IMotor* motor2;
    IEncoder* encoder1;
    IEncoder* encoder2;
    IMode* mode;
    int ledPin;
    unsigned long lastMovementTime;

public:
    PlotterSystem(IMotor* m1, IMotor* m2, IEncoder* e1, IEncoder* e2, IMode* md);

    void loop();
    void calibrate();
};