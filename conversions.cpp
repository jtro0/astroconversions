//
// Created by jtroo on 8-6-19.
//

#include "conversions.h"
#include "math.h"
#include "ctime"
#include "iostream"

#define INT(value) (value >= 0 ? (double)value : (double)(value-1))

// Astronomical Algorithms by Jean Meeus Page  94
void alt_az_to_ra_dec(double &ra, double &dec, double alt, double az, double lat, double &hour, double sidereal) {
    if (alt > M_PI/2) {
        alt = M_PI - alt;
        az += M_PI;
    } else if (alt < -M_PI/2) {
        alt = -M_PI - alt;
        az -= M_PI;
    }

    dec = asin(sin(lat)*sin(alt) + cos(lat)*cos(alt)*cos(az));

    hour = atan2(sin(az), (cos(az)*sin(lat) + tan(alt)*cos(lat)));

    ra = sidereal - hour;
}

void ra_dec_to_alt_az(double &alt, double &az, double ra, double dec, double lat, double &hour, double sidereal) {
    hour = sidereal - ra;

    az = atan2(sin(hour), cos(hour)*sin(lat) - tan(dec)*cos(lat));
    alt = asin(sin(lat)*sin(dec) + cos(lat)*cos(dec)*cos(hour));
}

double hms_to_decimal(int hours, int minutes, double seconds) {
    return hours + minutes/60 + seconds/3600;
}

void decimal_to_hms(int &hours, int &minutes, double &seconds, double decimal) {
    hours = (int)decimal;
    double remainder = (decimal-hours)*60; // minutes
    minutes = (int)remainder;
    seconds = (remainder-minutes)*60;
}

double hms_to_deg(int hours, int minutes, double seconds) {
    double decimal = hms_to_decimal(hours, minutes, seconds);

    return decimal*15;
}

void deg_to_hms(int &hours, int &minutes, double &seconds, double degree) {
    degree /= 15;
    decimal_to_hms(hours, minutes, seconds, degree);
}

double hms_to_rad(int hours, int minutes, double seconds) {
    double deg = hms_to_deg(hours, minutes, seconds);

    return deg*(M_PI/180);
}

double dms_to_decimal(int degree, int minutes, double seconds) {
    if (degree < 0) {
        minutes = -(int)fabs(minutes);
        seconds = -fabs(seconds);
    }

    return degree + minutes/60 + seconds/3600;
}

void decminal_to_dms(int &degree, int &minutes, double &seconds, double decimal) {
    degree = (int)decimal;
    double remainder = (decimal-degree)*60; // minutes
    minutes = (int)remainder;
    seconds = (remainder-minutes)*60;
}

double julianDay(int year, int month, double day) {
    if (month <= 2) {
        year--;
        month += 12;
    }

    int A = year/100;
    int B = 2-A+(A/4);
//    int B = -13;
    printf("A%d    B%d\n", A, B);
    return INT(365.25*(year+4716)) + INT(30.6001*(month+1)) + (int)day + B - 1524.5;
}

double jd_0_UT(double julianDay) {
    return (julianDay - 2451545)/36525;
}

double mean_sidereal_greenwich_decimal(double julianDay, bool normalized) {
    double a = 280.46061837 + 360.98564736629*(julianDay - 2451545);
    double T = jd_0_UT(julianDay);
    double b = 0.000387933*(T*T);
    double c = (T*T*T)/38710000;

    double theta = a + b - c;

    if (normalized) {
        if (theta < 0) {
            theta = 360.0 + fmod(theta, 360.0);
        }
    }
    return theta;
}

void mean_sidereal_greenwich_hour(int &hours, int &minutes, double &seconds, double julianDay) {
    double decimal = mean_sidereal_greenwich_decimal(julianDay, true);

    decimal_to_hms(hours, minutes, seconds, decimal);
}

// Use std::gmtime
double mean_sidereal_greenwich(std::tm time) {
    int year = time.tm_year + 1900;
    int month = time.tm_mon + 1;
    int day = time.tm_mday;
    int hour = time.tm_hour;
    int min = time.tm_min;
    int second = time.tm_sec;

    printf("%d/%d/%d %d:%d:%d\n", day, month, year, hour, min, second);
    double dayDouble = day;
    dayDouble += hms_to_decimal(hour, min, second)/24;

    double jd = julianDay(year, month, dayDouble);
    int hours, minutes;
    double seconds;
    mean_sidereal_greenwich_hour(hours, minutes, seconds, jd);
    printf("JD: %0.5f    %dh:%dm:%0.5fs\n", jd, hour, minutes, seconds);

    return mean_sidereal_greenwich_decimal(jd, true);
}

int main() {
    std::time_t t = std::time(0);
    std::tm* gmtnow = std::gmtime(&t);
    double sidereal = mean_sidereal_greenwich(*gmtnow);
    printf("%0.1f\n", sidereal);

    double jd = julianDay(1957, 10, 4.81);
    sidereal = mean_sidereal_greenwich_decimal(jd, true);
    printf("Julian: %0.5f    %0.1f\n", jd, sidereal);


}