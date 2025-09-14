#pragma once

#include "Arduino.h"
#include "IMode.h"
#include "AccelStepper.h"
#include "Settings.h"
#include "PolarSettings.h"


class PolarMode : public IMode {
    private:
    AccelStepper* stepper_left;
    AccelStepper* stepper_right;
    int canvasWidth, canvasHeight;
    int l_left, l_right; // current lengths of cables.

    float start_calibrate_switches(AccelStepper* as)
    {
        float max_speed = as->maxSpeed();
        as->setMaxSpeed(500);
        as->moveTo(-5000);
        return max_speed;
    }

    void finish_calibrate_switches(AccelStepper* as, float speed)
    {   
        as->setCurrentPosition(0);
        as->moveTo(0);
        as->setMaxSpeed(speed);
    }   

    void move_counterweights_to_switches()
    {
        float max_speed_left = start_calibrate_switches(stepper_left);
        float max_speed_right = start_calibrate_switches(stepper_right);

        while(digitalRead(LEFT_LIMIT_SW_PIN) || digitalRead(RIGHT_LIMIT_SW_PIN))
        {
            if(digitalRead(LEFT_LIMIT_SW_PIN))
                stepper_left->run();
            if(digitalRead(RIGHT_LIMIT_SW_PIN))
                stepper_right->run();
        }

        finish_calibrate_switches(stepper_left, max_speed_left);
        finish_calibrate_switches(stepper_right, max_speed_right);

    }

    void initial_move_cables_up()
    {
        stepper_left->moveTo(3000);
        stepper_right->moveTo(3000);
        
        while (stepper_left->run() || stepper_right->run())
        {
            stepper_left->run();
            stepper_right->run();
        }
    }

    public:

    PolarMode(AccelStepper* stepper_left, AccelStepper* stepper_right, int w, int h)
    :stepper_left(stepper_left),stepper_right(stepper_right),canvasWidth(w),canvasHeight(h),
    l_left(0),l_right(0)
    {
        pinMode(LEFT_LIMIT_SW_PIN, INPUT_PULLUP);
        pinMode(RIGHT_LIMIT_SW_PIN, INPUT_PULLUP);
    }
    /**
    given the current lengths of the belts 
    */
    Point calc_xy_from_polar(float l1, float l2)
    {
        const float d = DIST_BETWEEN_MOTORS;
        if (l1 + l2 <= d || fabs(l1 - l2) >= d)
            return Point{-1,-1};
        float x = (l1*l1 - l2*l2 + d*d) / (2.0f * d);
        float r2 = l1*l1 - x*x;
        if (r2 < -1e-6) 
            return Point{-1,-1};
        float y = sqrt(max(0.0f, r2));
        return Point{x,y};
    }
        
    
    void calibrate() override
    {
        initial_move_cables_up();
        move_counterweights_to_switches();
        stepper_left->setCurrentPosition(0);
        stepper_right->setCurrentPosition(0);
    }

    bool updateEndEffector(int d_left, int d_right) override
    {
        if(d_left == 0 && d_right == 0)
            return false;
        long n_left = l_left + d_left;
        long n_right = l_right + d_right;
        #if ENABLE_SOFT_LIMIT
        Point n = calc_xy_from_polar(n_left,n_right);
        if (n.x < 0 || n.x > canvasWidth || n.y < 0 || n.y > canvasHeight)
            return false; // out of bounds
        #endif
        l_left = n_left;
        l_right = n_right;
        stepper_left->moveTo(l_left);
        stepper_right->moveTo(l_right);
        return true;
    }
};