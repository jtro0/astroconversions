//
// Created by jtroo on 7-6-19.
//

#ifndef TEL_SIMPLEAHRS_H
#define TEL_SIMPLEAHRS_H

#include "limits.h"
#include "../sensors/adxl345.h"
#include "../sensors/l3g4200d.h"
#include "../sensors/hmc5883l.h"
#include "chrono"

class simpleAHRS {
private:
    float yaw;
    float pitch;
    float roll;

    adxl345 accel;
    l3g4200d gyro;
    hmc5883l magno;

    std::chrono::high_resolution_clock::time_point lastUpdate;

public:
    float getYaw() { return yaw; }
    float getPitch() { return  pitch; }
    float getRoll() { return roll; }

    simpleAHRS();
    simpleAHRS(adxl345&, l3g4200d&, hmc5883l&);

    void update();
};


#endif //TEL_SIMPLEAHRS_H
