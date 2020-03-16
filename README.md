# inelastic

This repository contains two different projects for the simulation of the formation of early solar systems. It generates 10,000 particles with random masses within a given range, and sizes which are functions of that mass. Then, they are placed in an anulus about a central mass with a momentum that would lead to circular motion with small amounts of randomness added to the motion. The data is then exported to a csv for visualization.

The python files were done in two dimensions. Because the program already took a large amount of time to run, I wanted to move the project in a language that would run faster, so I rewrote the necessary math in c++ for three dimsnions. It has a few parameters to play with, primarily the range of acceptable values and the number of initial dust particles moving in one direction vs. the other.

The interesting thing about this project is that the particles were allowed to perfectly inellastically collide (collide and stick together in such away that preserves momentum but not kinetic energy). This meant that as time progressed, we saw that there were fewer and fewer particles and they will increase in size. This demonstrates how planets can be formed by the collision of smaller solar masses and how allowing for inellastic collisions leads to a universe that preserves its total amount of angular momentum (which exaplins why all of the planets in our solar system orbit around the sun in the same direction).