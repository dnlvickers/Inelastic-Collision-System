#include <iostream>
#include "Dust.h"
#include <vector>
#include <cmath>
#include "Vec.h"
#include <fstream>

//Calculating the Force of Gravity
Vec fGravity(Dust mover, Dust actor, double G) {
    Vec rVector = actor.getPosition();
    rVector.subtract(mover.getPosition());
    double r = rVector.getMag();

    rVector.scalarMult(G * mover.getMass() * actor.getMass() / (r*r*r));

    return rVector;
}

int main() {
    srand(time(NULL));
    //...............................................................................
    //Length of Simulation
    int N1 = 6000; //number of particles spinning counter-clockwise
    int N2 = 4000; //number of particles spinning clockwise
    int N = N1 + N2; //number of particles
    double dt = 60.0*60.0*3.0; //length of time step
    int steps = 80000; //number of time steps
    int saveFrames = 4; //number of frames skipped before saving data

    //Star properties
    double starMass = 2e30;
    double G = 6.674e-11;
    Vec zero;
    zero.setComponents(0.0,0.0,0.0);

    //Range of Particles
    double r1 = 46.0e9; //definition of inner radius
    double r2 = 70.0e9; //definition of outer radius
    double zC = 0.65; //maximum z height to place the particles is scaled by this before squeezing, range 0.0-1.0

    //Particle eccentricity
    double eccentricity = 0.2; //percent deviation from expected velocity

    //Particle mass
    double massUnit = 8e23;
    double massEccentricity = 2.0;

    //...............................................................................
    //Creation of Particles
    Dust particles[N];

    for (int i = 0; i < N; ++i) {
        Dust piece;
        piece.setID(i);

        //creating dust location
        Vec r;
        double rmag = 0.0;
        while (rmag < r1 or rmag > r2) {
            double x = (((rand() % (int) 1e8) * 1e-8) - 0.5) * 2.0;
            double y = (((rand() % (int) 1e8) * 1e-8) - 0.5) * 2.0;
            double z = pow((((rand()%(int)1e8) * 1e-8) - 0.5) * 2.0 * zC,5.0);
            r.setComponents(x,y,z);
            r.scalarMult(r2);
            rmag = r.getMag();
        };
        piece.setPosition(r);

        //Generate mass
        double mass = (1.0 + (massEccentricity * ((rand()%(int)1e8) * 1e-8))) * massUnit;
        piece.setMass(mass);

        //Generating momentum
        Vec p;
        double x = r.getX();
        double y = r.getY();
        double z = r.getZ();
        double sinTheta = pow(((x*x) + (y*y)) / ((x*x) + (y*y) + (z*z)),1.0/2.0);
        double pScalar = pow((G * starMass / r.getMag()) * sinTheta,1.0/2.0);
        double px = (-(y/r2) + (eccentricity*2.0*(((rand()%(int)1e8) * 1e-8) - 0.5))) * pScalar * mass;
        double py = ((x/r2) + (eccentricity*2.0*(((rand()%(int)1e8) * 1e-8) - 0.5))) * pScalar * mass;
        double pz = (0.1 * (eccentricity*2.0*(((rand()%(int)1e8) * 1e-8) - 0.5))) * pScalar * mass;
        p.setComponents(px,py,pz);
        if (i >= N1){
            p.scalarMult(-1.0);
        }
        piece.setMomentumm(p);

        particles[i] = piece;
    }

    //Implement Data Collection
    std::ofstream dataFile;
    dataFile.open("/home/vickdj14/dnlvi/Documents/SystemOut.csv");
    dataFile << "ID,time,x,y,z,r\n";

    for (int l = 0; l < N; ++l) {
        dataFile << particles[l].getID() << ",";
        dataFile << 0 << ",";
        dataFile << particles[l].getPosition().getX() << ",";
        dataFile << particles[l].getPosition().getY() << ",";
        dataFile << particles[l].getPosition().getZ() << ",";
        dataFile << particles[l].getRadius() << "\n";
    }

    //Generation of Central Star
    Dust sun;
    sun.setMass(starMass);

    int solarCollisions = 0;
    int dustCollisions = 0;
    int infinityParticles = 0;

    std::cout << "Beginning Simulation" << std::endl;
    int count = 0;

    for (int k = 0; k < steps; ++k) {
        for (int j = 0; j < N; ++j) {
            //Calculate forces and move particles
            if(! particles[j].checkDestruction()) {
                Vec force = fGravity(particles[j],sun,G);
                for (int i = 0; i < N; ++i) {
                    if((! particles[i].checkDestruction()) and  i != j){
                        force.add(fGravity(particles[j],particles[i],G));
                    }
                }
                force.scalarMult(dt);
                force.add(particles[j].getMomentum());
                particles[j].setMomentumm(force);
                particles[j].step(dt);

                //Check for collisions
                Vec currentPos = particles[j].getPosition();
                double currentRad = particles[j].getRadius();

                //Particles at infinity
                if (currentPos.getMag() > 20 * r2) {
                    particles[j].destroy();
                    ++infinityParticles;
                    std::cout << "Particle at infinity No. " << infinityParticles << " at time " << (double)k*100.0/(double)steps << "%" << std::endl;
                }

                //Solar collision detection
                if (currentPos.getMag() < sun.getRadius()) {
                    sun.collide(particles[j]);
                    sun.setPosition(zero);
                    particles[j].destroy();
                    ++solarCollisions;
                    std::cout << "Solar collision No. " << solarCollisions << " at time " << (double)k*100.0/(double)steps << "%" << std::endl;
                    continue;
                }

                //checking for collisions with other particles
                for (int i = 0; i < j; ++i) {
                    if((! particles[i].checkDestruction()) and  i != j) {
                        Vec posDiff = particles[i].getPosition();
                        posDiff.subtract(currentPos);

                        if(posDiff.getMag() < currentRad + particles[i].getRadius()){
                            particles[j].collide(particles[i]);
                            particles[i].destroy();
                            ++dustCollisions;
                            std::cout << "Dust collision No. " << dustCollisions << " at time " << (double)k*100.0/(double)steps << "%" << std::endl;
                        }
                    }
                }
            }
        }
        ++count;

        //write data
        if(count == saveFrames) {
            for (int l = 0; l < N; ++l) {
                if (!particles[l].checkDestruction()) {
                    dataFile << particles[l].getID() << ",";
                    dataFile << k * dt << ",";
                    dataFile << particles[l].getPosition().getX() << ",";
                    dataFile << particles[l].getPosition().getY() << ",";
                    dataFile << particles[l].getPosition().getZ() << ",";
                    dataFile << particles[l].getRadius() << "\n";
                }
            }
            count = 0;
        }
    }
    //close data file and end program
    dataFile.close();
    std::cout << "Data Complete\n";
    std::cout << "Solar Collisions: " << solarCollisions << std::endl;
    std::cout << "Dust Collisions: " << dustCollisions << std::endl;
    std::cout << "Particles at Infinity: " << infinityParticles << std::endl;
    std::cout << "Total Collisions: " << solarCollisions + dustCollisions + infinityParticles << std::endl;
    std::cout << "Particles remaining: " << N - solarCollisions - dustCollisions - infinityParticles << std::endl;

    return 0;
}