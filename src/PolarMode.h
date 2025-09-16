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
    int l_left, l_right; // current lengths of cables.

    void start_calibrate_switches(AccelStepper* as)
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
        Serial.println("moving counterweights to switches");
        start_calibrate_switches(stepper_left);
        start_calibrate_switches(stepper_right);
        bool left_at_target = false;
        bool right_at_target = false;
        while(digitalRead(LEFT_LIMIT_SW_PIN) || digitalRead(RIGHT_LIMIT_SW_PIN))
        {
            if(digitalRead(LEFT_LIMIT_SW_PIN))
                stepper_left->run();
            if(digitalRead(RIGHT_LIMIT_SW_PIN))
                stepper_right->run();
        }
        finish_calibrate_switches(stepper_left);
        finish_calibrate_switches(stepper_right);

    }

    void initial_move_cables_up()
    {
        Serial.println("moving cables up");
        stepper_left->moveTo(3000);
        stepper_right->moveTo(3000);
        
        while (stepper_left->run() || stepper_right->run())
        {
            stepper_left->run();
            stepper_right->run();
        }
    }

    /// @brief test if the given coords are in the legal area (defined in settings.h), with a possible margin around the legal area acting as a deadband (bc of possible jittery input).
    bool is_within_bounds(long x, long y, int margin=0)
    {
        if (x < (POLAR_X_MIN_LIMIT + margin) || x > (POLAR_X_MAX_LIMIT - margin) || y < (POLAR_Y_MIN_LIMIT + margin) || y > (POLAR_Y_MAX_LIMIT - margin))
            return false;
        return true;
    }

    public:

    PolarMode(AccelStepper* stepper_left, AccelStepper* stepper_right)
    :stepper_left(stepper_left),stepper_right(stepper_right), l_left(0),l_right(0)
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
            return Point{-10000,-10000};
        float x = (l1*l1 - l2*l2 + d*d) / (2.0f * d);
        float r2 = l1*l1 - x*x;
        if (r2 < -1e-6) 
            return Point{-10000,-10000};
        float y = sqrt(max(0.0f, r2));
        return Point{int(x),int(y)};
    }

    Point calc_polar_from_xy(int x, int y)
    {
        //todo: implement.
        // double h = (-1 * y) - Y_OFFSET;
        // double w = -1 * (x - X_OFFSET - double(MOTORS_DISTANCE));
        // int l2 = int(sqrt(h * h + w * w)) * LEFT_STEPS_PER_MM;
        // w = w - double(MOTORS_DISTANCE);
        // int l1 = int(sqrt(h * h + w * w)) * RIGHT_STEPS_PER_MM;
        // return Point{l1,l2};
        return Point{0,0};
    }
        
    
    void calibrate() override
    {
        #if DEBUG_MODE
        finish_calibrate_switches(stepper_left);
        finish_calibrate_switches(stepper_right);
        #else
        initial_move_cables_up();
        move_counterweights_to_switches();
        #endif
    }

    bool updateEndEffector(int d_left, int d_right) override
    {
        if(d_left == 0 && d_right == 0)
            return false;
        long n_left = l_left + d_left;
        long n_right = l_right + d_right;
        #if DEBUG_MODE
        Serial.println(n_left);
        Serial.println(n_right);
        #endif
        #if ENABLE_SOFT_LIMIT
        Point n = calc_xy_from_polar(n_left,n_right);
        if(!is_within_bounds(n.x,n.y))
            return false;
        #endif
        
        l_left = n_left;
        l_right = n_right;

        #if ENABLE_SOFT_LIMIT
        // deadband test - if is within deadband update coords but don't move to the edge.
        if(!is_within_bounds(n.x,n.y,5))
            return false;
        #endif

        stepper_left->moveTo(l_left * STEPPER_STEPSIZE);
        stepper_right->moveTo(l_right * STEPPER_STEPSIZE);
        return true;
    }

    Point get_motor_lengths()
    {
        return Point{l_left,l_right};
    }

    void go_home() override
    {
        stepper_left->moveTo(POLAR_LEFT_HOME);
        stepper_right->moveTo(POLAR_RIGHT_HOME);
        while(stepper_left->run() || stepper_right->run())
        {
            stepper_left->run();
            stepper_right->run();
        }
        l_left = 0;
        l_right = 0;
    }
};