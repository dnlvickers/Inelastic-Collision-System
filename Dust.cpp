//
// Created by dnlvi on 12/17/2018.
//

#include "Dust.h"
#include <cmath>
#include "Vec.h"
#include <iostream>

Dust::Dust() {
    position.setComponents(0.0,0.0,0.0);
    momentum.setComponents(0.0,0.0,0.0);

    mass = 1.0;
    radius = pow(mass,1.0 / 3.0);
    collision = false;

    ID = 0;
}

double Dust::getMass() {
    return mass;
}

double Dust::getRadius() {
    return radius;
}

Vec Dust::getPosition() {
    return position;
}

Vec Dust::getMomentum() {
    return momentum;
}

void Dust::setMass(double m) {
    mass = m;
    radius = pow(mass, 1.0 / 3.0);
}

void Dust::setPosition(Vec pos) {
    position = pos;
}

void Dust::setMomentumm(Vec mom) {
    momentum = mom;
}

void Dust::step(double dt) {
    Vec pSubstitute = momentum;
    pSubstitute.scalarMult(1.0/mass);
    pSubstitute.scalarMult(dt);
    position.add(pSubstitute);
}

void Dust::collide(Dust d2) {
    //Place position at center of mass
    double otherMass = d2.getMass();
    Vec otherPos = d2.getPosition();
    Vec CoM = position;
    otherPos.scalarMult(otherMass);
    CoM.scalarMult(mass);
    CoM.add(otherPos);
    CoM.scalarMult(1.0/(otherMass+mass));
    position = CoM;

    /*position.add(d2.getPosition());
    position.scalarMult(1.0/2.0);*/

    //Add momentum, mass, and update radius
    momentum.add(d2.getMomentum());
    mass += otherMass;
    radius = pow(mass,1.0 / 3.0);
}

void Dust::destroy() {
    collision = true;
}

bool Dust::checkDestruction() {
    return collision;
}

void Dust::setID(int name) {
    ID = name;
}

int Dust::getID() {
    return ID;
}