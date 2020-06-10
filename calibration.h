//
// Created by jtroo on 8-6-19.
//

#ifndef TEL_CALIBRATION_H
#define TEL_CALIBRATION_H

#include "../sensors/adxl345.h"
#include "../sensors/l3g4200d.h"
#include "../sensors/hmc5883l.h"

#include "iostream"
class calibration {
private:
    adxl345 acc;
    l3g4200d gyro;
    hmc5883l magno;

public:
    calibration(adxl345&, l3g4200d&, hmc5883l&);
    void calibrate(bool, bool);
    void calibrateGyro(int);
};


#endif //TEL_CALIBRATION_H
