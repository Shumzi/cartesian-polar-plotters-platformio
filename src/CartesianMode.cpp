#include "Arduino.h"
#include "IMode.h"
#include "IMotor.h"
#include "Settings.h"
#include "CartesianSettings.h"
#include "AccelStepper.h"

class CartesianMode : public IMode 
{
    private:
        AccelStepper stepper_x;
        AccelStepper stepper_y;
        long canvasWidth, canvasHeight;
        long x, y; // current end effector position
    public:
        CartesianMode(AccelStepper xm, AccelStepper ym, long w, long h)
            : stepper_x(xm), stepper_y(ym), canvasWidth(w), canvasHeight(h), x(0), y(0) {}

        bool updateEndEffector(int dx, int dy) override 
        {
            long nx = x + dx;
            long ny = y + dy;
            if (nx < 0 || nx > canvasWidth || ny < 0 || ny > canvasHeight)
                return false; // out of bounds
            x = nx;
            y = ny;
            stepper_x.moveTo(x);
            stepper_y.moveTo(y);
            return true;
        }

        void calibrate() override
        {
            Serial.println("homing");
            Serial.print("xMotor loc");
            Serial.println(stepper_x.currentPosition());
            // pin to left
            stepper_x.moveTo(stepper_x.currentPosition() - (X_MAX_LIMIT + 100 - X_MIN_LIMIT) * STEPPER_X_STEPSIZE);
            float acc = stepper_x.acceleration();
            float max_speed = stepper_x.maxSpeed();
            stepper_x.setAcceleration(100);
            stepper_x.setMaxSpeed(1000);
            stepper_x.runToPosition();
            // move to middle
            stepper_x.setAcceleration(acc);
            stepper_x.setMaxSpeed(max_speed);
            stepper_x.moveTo(stepper_x.currentPosition() + X_HOMING_OFFSET * STEPPER_X_STEPSIZE);
            stepper_x.runToPosition();
            Serial.println("moved x");
            // move to bottom
            stepper_y.disableOutputs();
            Serial.println("disabled y");
            delay(2000);
            stepper_y.enableOutputs();
            Serial.println("enabled y");
            // move to middle
            stepper_y.moveTo(stepper_y.currentPosition() + Y_HOMING_OFFSET * STEPPER_Y_STEPSIZE);
            stepper_y.runToPosition();
            stepper_x.setCurrentPosition(0);
            stepper_y.setCurrentPosition(0);
        }
};