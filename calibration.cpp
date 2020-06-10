//
// Created by jtroo on 8-6-19.
//

#include "calibration.h"


calibration::calibration(adxl345 &accelerometer, l3g4200d &gyroscope, hmc5883l &magnetometer) {
    acc = accelerometer;
    gyro = gyroscope;
    magno = magnetometer;
}

void calibration::calibrate(bool accelerometer, bool magnetometer) {

    float accXMin = 0;
    float accXMax = 0;
    float accYMin = 0;
    float accYMax = 0;
    float accZMin = 0;
    float accZMax = 0;

    float magXMin = 0;
    float magXMax = 0;
    float magYMin = 0;
    float magYMax = 0;
    float magZMin = 0;
    float magZMax = 0;

    printf("Calibrating in... (Move slowly)\n");
    printf("3\n");
    sleep(1);
    printf("2\n");
    sleep(1);
    printf("1\n");
    sleep(1);

    for (int i=0; i<500; i++) {
        if (accelerometer) {
            acc.update(true);
            float ax = acc.getX();
            float ay = acc.getY();
            float az = acc.getZ();

            if (ax < accXMin)
                accXMin = ax;
            else if (ax > accXMax)
                accXMax = ax;

            if (ay < accYMin)
                accYMin = ay;
            else if (ay > accYMax)
                accYMax = ay;

            if (az < accZMin)
                accZMin = az;
            else if (az > accZMax)
                accZMax = az;
        }

        if (magnetometer) {
            magno.update(true);
            float mx = magno.getX();
            float my = magno.getY();
            float mz = magno.getZ();

            if (mx < magXMin)
                magXMin = mx;
            else if (mx > magXMax)
                magXMax = mx;

            if (my < magYMin)
                magYMin = my;
            else if (my > magYMax)
                magYMax = my;

            if (mz < magZMin)
                magZMin = mz;
            else if (mz > magZMax)
                magZMax = mz;
        }

        printf("%d\r", i);

        usleep(100000);
    }

    if (accelerometer) {
        float axOffset = (accXMax+accXMin)/2;
        float ayOffset = (accYMax+accYMin)/2;
        float azOffset = (accZMax+accZMin)/2;

        acc.setOffsets(axOffset, ayOffset, azOffset);
    }

    if (magnetometer) {
        float mxOffset = (magXMax+magXMin)/2;
        float myOffset = (magYMax+magYMin)/2;
        float mzOffset = (magZMax+magZMin)/2;

        magno.setOffsets(mxOffset, myOffset, mzOffset);
    }

    printf("\nDone!\n");
}

void calibration::calibrateGyro(int samples) {
    float xTotal = 0;
    float yTotal = 0;
    float zTotal = 0;

    printf("Calibrating in... (Don't move!)\n");
    printf("3\n");
    sleep(1);
    printf("2\n");
    sleep(1);
    printf("1\n");
    sleep(1);

    for (int i=0; i<samples; i++) {
        gyro.update(true);

        float gx = gyro.getX();
        float gy = gyro.getY();
        float gz = gyro.getZ();

        xTotal += gx;
        yTotal += gy;
        zTotal += gz;
//        std::cout << i << std::endl;
        printf("%d\r", i);
        usleep(1000);
    }

    gyro.setOffsets(xTotal/samples, yTotal/samples, zTotal/samples);

    printf("\nDone!\n");
}
