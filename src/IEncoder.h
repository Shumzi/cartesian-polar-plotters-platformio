#pragma once

class IEncoder {
public:
    virtual int readDelta() = 0;
    virtual ~IEncoder() {}
};
