#pragma once

#include <Arduino.h>
#include "AccelStepper.h"
#include "RotaryEncoder.h"
#include "Settings.h"
#include "IEncoder.h"
#include "IMode.h"

class PlotterSystem {
private:
    AccelStepper* motor1;
    AccelStepper* motor2;
    IEncoder* encoder1;
    IEncoder* encoder2;
    IMode* mode;
    bool uv_state = false;
    unsigned long lastMovementTime;
    // int encoder_1_register[4] = {0,0,0,0};
    // int encoder_2_register[4] = {0,0,0,0};
    // int encoder_1_register_loc = 0;
    // int encoder_2_register_loc = 0;
    
    void update_uv()
    {
        lastMovementTime = millis();
        if (!uv_state)
        {
            uv_state = HIGH;
            digitalWrite(UV_PIN, uv_state);
        }
    }

    void check_idle()
    {
        if (uv_state && ((millis() - lastMovementTime) > UV_AUTO_TURN_OFF_TIME))
        {
            uv_state = LOW;
            digitalWrite (UV_PIN, uv_state);
            lastMovementTime = millis();
        }
    }

public:
    PlotterSystem(AccelStepper* m1, AccelStepper* m2, IEncoder* e1, IEncoder* e2, IMode* md)
    : motor1(m1), motor2(m2), encoder1(e1), encoder2(e2), mode(md), lastMovementTime(millis()) 
    { 
        pinMode(UV_PIN, OUTPUT);
    }
        
    void loop() 
    {
        int delta1 = encoder1->readDelta();
        int delta2 = encoder2->readDelta();
        // if (delta1 != 0)
        //     encoder_1_register[encoder_1_register_loc];
        // if (delta2 != 0)
        //     encoder_2_register[encoder_2_register_loc];
        bool moved = mode->updateEndEffector(delta1, delta2);
        motor1->run();
        motor2->run();

        if(moved)
            update_uv();
        else
            check_idle();
    }

    void calibrate() { mode->calibrate(); }
};