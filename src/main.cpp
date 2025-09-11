#include <Arduino.h>
#include <AccelStepper.h>
#include "RotaryEncoder.h"
#include "PolarMode.h"
#include "CartesianMode.h"
#include "Settings.h"
#include "PlotterSystem.h"

// DEFINITIONS:
void print_current_position();

AccelStepper stepper_1(AccelStepper::DRIVER, STEP_1_PIN, DIR_1_PIN);
AccelStepper stepper_2(AccelStepper::DRIVER, STEP_2_PIN, DIR_2_PIN);
#if USE_POLAR_MODE
Mode mode = new PolarMod
#else

#endif
// USER INTERFACE OBJECTS

RotaryEncoder encoder_1 = RotaryEncoder(ENCODER_A_BIT_0, ENCODER_A_BIT_1, ENCODER_A_BUTTON);
RotaryEncoder encoder_2 = RotaryEncoder(ENCODER_B_BIT_0, ENCODER_B_BIT_1, ENCODER_B_BUTTON);
PlotterSystem p = PlotterSystem(stepper_1, stepper_2, encoder_1, encoder_2, )
unsigned long time_last_action = 0;
bool uv_state = LOW;





void print_current_position()
{
    Serial.println("Position: ");
    Serial.print(stepper_x.currentPosition());
    Serial.print(",");
    Serial.println(stepper_y.currentPosition());

}

void setup()
{

    Serial.begin(115200);
    /** Init Joystick input pins **/
    /** AUTO HOME**/
    // auto_homing(&stepper_c);
    pinMode (UV_PIN, OUTPUT);
    
    stepper_x.setMaxSpeed(X_MAX_SPEED * STEPPER_X_STEPSIZE);
    stepper_x.setAcceleration(X_ACCELERATION * STEPPER_X_STEPSIZE);

    stepper_y.setMaxSpeed(Y_MAX_SPEED * STEPPER_Y_STEPSIZE);
    stepper_y.setAcceleration(Y_ACCELERATION * STEPPER_Y_STEPSIZE);
    stepper_y.setEnablePin(EN_PIN);
    stepper_y.setPinsInverted(false,false,true);
    stepper_y.enableOutputs();


}


void update_uv_movement()
{
    time_last_action = millis();
    if(!uv_state)
    {
        uv_state = HIGH;
        digitalWrite (UV_PIN, uv_state);
    }
}

void check_idle()
{
    if (uv_state && ((millis() - time_last_action) > PEN_AUTO_TURN_OFF_TIME)
    {
        uv_state = LOW;
        digitalWrite (UV_PIN, uv_state);
        time_last_action = millis();
    }
}

void loop()
{
    
}
