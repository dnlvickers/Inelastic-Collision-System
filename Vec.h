//
// Created by dnlvi on 12/17/2018.
//

#ifndef DUST_VEC_H
#define DUST_VEC_H


class Vec {
public:
    Vec();
    void setComponents(double i, double j, double k);
    double getMag();
    double getX();
    double getY();
    double getZ();

    void add(Vec v1);
    void subtract(Vec v1);
    void scalarMult(double c);
private:
    double x,y,z;
};


#endif //DUST_VEC_H
