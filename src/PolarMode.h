#pragma once

#include "Arduino.h"
#include "IMode.h"
#include "AccelStepper.h"
#include "Settings.h"
#include "PolarSettings.h"


class PolarMode : public IMode {
    private:
    AccelStepper stepper_left;
    AccelStepper stepper_right;
    
    public:

    PolarMode(AccelStepper stepper_left, AccelStepper stepper_right)
    :stepper_left(stepper_left),stepper_right(stepper_right) {}
    /**
    given the current lengths of the belts 
    */
    Point calc_xy_from_polar(float l1, float l2)
    {
        const float d = DIST_BETWEEN_MOTORS;
        if (l1 + l2 <= d || fabs(l1 - l2) >= d)
            raise ValueError("No intersection: lengths incompatible");
        float x = (l1*l1 - l2*l2 + d*d) / (2.0f * d);
        float r2 = l1*l1 - x*x;
        if (r2 < -1e-6) 
            throw error;
        float y = sqrt(max(0.0f, r2));
        return Point(x,y);
    }
        
    void move_to_switches()
    {
        float max_speed_x = stepper_left.maxSpeed();
        float max_speed_y = stepper_right.maxSpeed();
        
        stepper_left.setMaxSpeed(500);
        stepper_right.setMaxSpeed(500);
        stepper_left.moveTo(-5000);
        stepper_right.moveTo(-5000);
        
        while(digitalRead(LEFT_LIMIT_SW_PIN) || digitalRead(RIGHT_LIMIT_SW_PIN))
        {
            if(digitalRead(LEFT_LIMIT_SW_PIN))
                stepper_left.run();
            if(digitalRead(RIGHT_LIMIT_SW_PIN))
                stepper_right.run();
        }
        stepper_left.setCurrentPosition(0);
        stepper_left.moveTo(0);
        stepper_left.setMaxSpeed(max_speed_x);
        stepper_right.setCurrentPosition(0);
        stepper_right.moveTo(0);
        stepper_right.setMaxSpeed(max_speed_y);
    }

    void calibrate() override
    {
        pinMode(LEFT_LIMIT_SW_PIN, INPUT_PULLUP);
        pinMode(RIGHT_LIMIT_SW_PIN, INPUT_PULLUP);
        stepper_left.moveTo(3000);
        stepper_right.moveTo(3000);
        while(stepper_left.run() || stepper_right.run())
        {
            stepper_left.run();
            stepper_right.run();
        }
        move_to_switches();
        stepper_left.setCurrentPosition(0);
        stepper_right.setCurrentPosition(0);
    }

    bool updateEndEffector(int delta1, int delta2) override
    {

    }
};