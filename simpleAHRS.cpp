//
// Created by jtroo on 7-6-19.
//

#include "simpleAHRS.h"
#include "iostream"


simpleAHRS::simpleAHRS() {
    std::cout << "INIT ACCEL" << std::endl;
    accel = adxl345();
    accel.init();
//    accel.calibrate();

    magno = hmc5883l();
    magno.init();

    std::cout << "INIT GYRO" << std::endl;
    gyro = l3g4200d();
    gyro.init();
//    gyro.calibrate();


    roll = 0;
    pitch = 0;
    yaw = 0;
    lastUpdate = std::chrono::high_resolution_clock::now();
};

simpleAHRS::simpleAHRS(adxl345 &accelerometer, l3g4200d &gyroscope, hmc5883l &magnetometer) {
    accel = accelerometer;
    gyro = gyroscope;
    magno = magnetometer;

    roll = 0;
    pitch = 0;
    yaw = 0;
    lastUpdate = std::chrono::high_resolution_clock::now();
}

void simpleAHRS::update() {
    accel.update(true);
    float ax = accel.getX();
    float ay = accel.getY();
    float az = accel.getZ();
    float aPitch = accel.getPitch();
    float aRoll = accel.getRoll();
    float aYaw = accel.getYaw();
//    printf("UPDATE Ax: %0.2f    Ay: %0.2f    Az: %0.2f    PITCH: %0.2f    ROLL: %0.2f\n", ax, ay, az, aPitch, aRoll);

    magno.update(true);
    float mx = magno.getX();
    float my = magno.getY();
    float mz = magno.getZ();
    float mPitch = magno.getPitch();
    float mRoll = magno.getRoll();
    float mYaw = magno.getYaw();

//    printf("UPDATE Mx: %0.2f    My: %0.2f    Mz: %0.2f\n", mx, my, mz);

    gyro.update(true);
//    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

    float gx = gyro.getX();
    float gy = gyro.getY();
    float gz = gyro.getZ();

//    printf("UPDATE gx: %0.2f    gy: %0.2f    gz: %0.2f\n", gx, gy, gz);

    auto gyroLast = gyro.lastUpdate();
//    float dt = ( (unsigned long)gyroLast - lastUpdate)/CLOCKS_PER_SEC;
    std::chrono::duration<float > dt = std::chrono::duration_cast<std::chrono::duration<float>>(gyroLast - lastUpdate);
//    printf("!dt: %0.5f    gyrolast: %0.5f\n", dt.count());

    lastUpdate = gyroLast;

//    printf("dt: %0.5f    gyrolast: %0.5f\n", dt.count());

//    printf("ROLL: %0.5f    PITCH: %0.5f\n", roll, pitch);
    float roll1 = 0.95f*(roll + gx*dt.count());
    float roll2 = 0.05f*(0.9f*aRoll + 0.1f*mRoll);
    roll = roll1 + roll2;


    float  pitch1 = 0.95f*(pitch + gy*dt.count());
    float pitch2 = 0.05f*(0.9f*aPitch + 0.1f*mPitch);
    pitch = pitch1 + pitch2;

    float  yaw1 = 0.95f*(yaw + gz*dt.count());
    float yaw2 = 0.05f*(0.2f*aYaw + 0.8f*mYaw);
    yaw = yaw1 + yaw2;


//    printf ("roll1: %0.5f    roll2: %0.5f    pitch1: %0.5f    pitch2: %0.5f\n", roll1, roll2, pitch1, pitch2);

//    roll = 0.95f*(roll + gx*dt.count()) + 0.05f*aRoll;
//    pitch = 0.95f*(pitch + gy*dt.count()) + 0.05f*aPitch;
//    yaw = 0.95f*(yaw + gz*dt.count()) + 0.05f*aYaw;
//    sleep(1);
}