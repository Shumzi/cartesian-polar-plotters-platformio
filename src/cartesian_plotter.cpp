#include <Arduino.h>
#include <AccelStepper.h>
#include "RotaryEncoder.h"
#include "Settings.h"
#include "CartesianSettings.h"

// DEFINITIONS:
void print_current_position();

AccelStepper stepper_x(AccelStepper::DRIVER, STEP_1_PIN, DIR_1_PIN);
AccelStepper stepper_y(AccelStepper::DRIVER, STEP_2_PIN, DIR_2_PIN);
int current_x_pos = 0;
int current_y_pos = 0;

// USER INTERFACE OBJECTS

RotaryEncoder encoder_x = RotaryEncoder(ENCODER_A_BIT_0, ENCODER_A_BIT_1, ENCODER_A_BUTTON);
RotaryEncoder encoder_y = RotaryEncoder(ENCODER_B_BIT_0, ENCODER_B_BIT_1, ENCODER_B_BUTTON);
unsigned long time_last_action = 0;
bool uv_state = LOW;



void move_to_switches()
{
    float max_speed_x = stepper_x.maxSpeed();
    float max_speed_y = stepper_y.maxSpeed();
    
    stepper_x.setMaxSpeed(500);
    stepper_y.setMaxSpeed(500);
    stepper_x.moveTo(-5000);
    stepper_y.moveTo(-5000);
    
    while(digitalRead(X_LIMIT_SW_PIN) || digitalRead(Y_LIMIT_SW_PIN))
    {
        if(digitalRead(X_LIMIT_SW_PIN))
            stepper_x.run();
        if(digitalRead(Y_LIMIT_SW_PIN))
            stepper_y.run();
    }
    stepper_x.setCurrentPosition(0);
    stepper_x.moveTo(0);
    stepper_x.setMaxSpeed(max_speed_x);
    stepper_y.setCurrentPosition(0);
    stepper_y.moveTo(0);
    stepper_y.setMaxSpeed(max_speed_y);
}


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
    if(SYSTEM_TYPE == "cartesian")
        cartesian_auto_homing();
    else if(SYSTEM_TYPE == "polar")
        polar_extra_setup_and_auto_homing();

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
