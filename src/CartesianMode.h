#pragma once

#include <Arduino.h>
#include "IMode.h"
#include "AccelStepper.h"
#include "Settings.h"
#include "CartesianSettings.h"

class CartesianMode : public IMode 
{
    private:
        AccelStepper* stepper_x;
        AccelStepper* stepper_y;
        int canvasWidth, canvasHeight;
        int x, y; // current end effector position
    public:
        CartesianMode(AccelStepper* xm, AccelStepper* ym, long w, long h)
            : stepper_x(xm), stepper_y(ym), canvasWidth(w), canvasHeight(h), x(0), y(0)
            {
                stepper_x->setMaxSpeed(X_MAX_SPEED * STEPPER_STEPSIZE);
                stepper_x->setAcceleration(X_ACCELERATION * STEPPER_STEPSIZE);
                stepper_y->setMaxSpeed(Y_MAX_SPEED * STEPPER_STEPSIZE);
                stepper_y->setAcceleration(Y_ACCELERATION * STEPPER_STEPSIZE);
                stepper_y->setEnablePin(EN_PIN);
                stepper_y->setPinsInverted(false,false,true);
                stepper_y->enableOutputs();
            }
        
        void test()
        {
            Serial.println("test from Cartesian Mode");
        }

        bool updateEndEffector(int dx, int dy) override 
        {
            if(dx == 0 && dy == 0)
                return false;
            long nx = x + dx * STEPPER_STEPSIZE;
            long ny = y + dy * STEPPER_STEPSIZE;
            #if ENABLE_SOFT_LIMIT
            if (nx < 0 || nx > canvasWidth || ny < 0 || ny > canvasHeight)
                return false; // out of bounds
            #endif
            x = nx;
            y = ny;
            stepper_x->moveTo(x);
            stepper_y->moveTo(y);
            #if DEBUG_MODE
            Serial.println("delta:");
            Serial.println(dx);
            Serial.println(dy);
            Serial.print("moving to");
            Serial.println(x);
            Serial.println(y);
            #endif
            return true;
        }

        void calibrate() override
        {
            // Serial.println("not calibrating now");
            // return;
            Serial.println("homing");
            Serial.print("xMotor loc");
            Serial.println(stepper_x->currentPosition());
            // pin to left
            stepper_x->moveTo(stepper_x->currentPosition() - (X_MAX_LIMIT + 100 - X_MIN_LIMIT) * STEPPER_STEPSIZE);
            float acc = stepper_x->acceleration();
            float max_speed = stepper_x->maxSpeed();
            stepper_x->setAcceleration(100);
            stepper_x->setMaxSpeed(1000);
            stepper_x->runToPosition();
            // move to middle
            stepper_x->setAcceleration(acc);
            stepper_x->setMaxSpeed(max_speed);
            stepper_x->moveTo(stepper_x->currentPosition() + X_HOMING_OFFSET * STEPPER_STEPSIZE);
            stepper_x->runToPosition();
            Serial.println("moved x");
            // move to bottom
            stepper_y->disableOutputs();
            Serial.println("disabled y");
            delay(2000);
            stepper_y->enableOutputs();
            Serial.println("enabled y");
            // move to middle
            stepper_y->moveTo(stepper_y->currentPosition() + Y_HOMING_OFFSET * STEPPER_STEPSIZE);
            stepper_y->runToPosition();
            stepper_x->setCurrentPosition(0);
            stepper_y->setCurrentPosition(0);
        }
        
    Point get_motor_lengths()
    {
        return Point{x,y};
    }

    void go_home() override
    {
        stepper_x->moveTo(0);
        stepper_y->moveTo(0);
        while(stepper_x->run() || stepper_y->run())
        {
            stepper_x->run();
            stepper_y->run();
        }
        x = 0;
        y = 0;
    }
};