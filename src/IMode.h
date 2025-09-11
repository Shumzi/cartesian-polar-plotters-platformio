class IMode {
public:
    virtual bool updateEndEffector(int delta1, int delta2) = 0; // movement
    // virtual std::pair<long,long> getMotorTargets() = 0;
    virtual void calibrate() = 0; // calibration inside the mode
    virtual ~IMode() {}
};