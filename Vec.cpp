//
// Created by dnlvi on 12/17/2018.
//

#include "Vec.h"
#include <cmath>

Vec::Vec() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

void Vec::setComponents(double i, double j, double k) {
    x = i;
    y = j;
    z = k;
}

double Vec::getMag() {
    return pow((x*x)+(y*y)+(z*z),1.0/2.0);
}

double Vec::getX() {
    return x;
}

double Vec::getY() {
    return y;
}

double Vec::getZ() {
    return z;
}

void Vec::add(Vec v1) {
    x += v1.getX();
    y += v1.getY();
    z += v1.getZ();
}

void Vec::subtract(Vec v1) {
    x -= v1.getX();
    y -= v1.getY();
    z -= v1.getZ();
}

void Vec::scalarMult(double c) {
    x = x*c;
    y = y*c;
    z = z*c;
}