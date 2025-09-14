#pragma once

class IMode {
    public:
        virtual bool updateEndEffector(int delta1, int delta2) = 0; // movement
        virtual void calibrate() = 0; // calibration inside the mode]
};