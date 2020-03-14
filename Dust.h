//
// Created by dnlvi on 12/17/2018.
//

#ifndef DUST_DUST_H
#define DUST_DUST_H

#include <cmath>
#include "Vec.h"


class Dust {
public:
    Dust();
    Vec getPosition();
    Vec getMomentum();
    double getMass();
    double getRadius();

    void setPosition(Vec pos);
    void setMomentumm(Vec mom);
    void setMass(double newMass);

    void step(double dt);

    void collide(Dust d2);
    void destroy();
    bool checkDestruction();

    void setID(int name);
    int getID();
private:
    double mass, radius;
    Vec position;
    Vec momentum;
    bool collision;
    int ID;
};


#endif //DUST_DUST_H
