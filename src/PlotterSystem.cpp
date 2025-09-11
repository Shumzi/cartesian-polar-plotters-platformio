#include <Arduino.h>
#include "PlotterSystem.h"
#include <AccelStepper.h>
#include "RotaryEncoder.h"
#include "Settings.h"

PlotterSystem::PlotterSystem(IMotor* m1, IMotor* m2, IEncoder* e1, IEncoder* e2, IMode* md)
    : motor1(m1), motor2(m2), encoder1(e1), encoder2(e2), mode(md)
{
    lastMovementTime = millis();
    pinMode(ledPin, OUTPUT);

}

void PlotterSystem::loop() {
    int delta1 = encoder1->readDelta();
    int delta2 = encoder2->readDelta();
    mode->updateEndEffector(delta1, delta2);

    // turn off LED if idle.
    check_idle();

    motor1->run();
    motor2->run();


    int new_x_pos = encoder_x.read_encoder();
    int new_y_pos = encoder_y.read_encoder();

    // update_UV(encoder_x, encoder_y);
    print_when_press(encoder_x, encoder_y);
    home_when_press(encoder_x);
    // calculate move X
    if(current_x_pos != new_x_pos && new_x_pos > X_MIN_LIMIT + 5 && new_x_pos < X_MAX_LIMIT - 5)
    {   
        stepper_x.moveTo(new_x_pos * STEPPER_X_STEPSIZE);
        current_x_pos = new_x_pos;
        update_uv_movement();
    }
    
    // calculate move Y
    if(current_y_pos != new_y_pos && new_y_pos > Y_MIN_LIMIT + 5 && new_y_pos < Y_MAX_LIMIT - 5)
    {   
        stepper_y.moveTo(new_y_pos * STEPPER_X_STEPSIZE);
        current_y_pos = new_y_pos;
        update_uv_movement();
    }
    


}

void PlotterSystem::calibrate() {
    mode->calibrate();
}