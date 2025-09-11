#pragma once

class IMotor {
public:
    virtual void moveTo(long position) = 0;
    virtual long currentPosition() = 0;
    virtual void setSpeed(float speed) = 0;
    virtual void run() = 0;
    virtual ~IMotor() {}
};