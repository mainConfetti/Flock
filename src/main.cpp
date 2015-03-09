#include <iostream>
#include <stdlib.h>
#include <boid.h>
#include <flock.h>

int main()
{
    std::cout << "Hello World!" << std::endl;
    // Test Boid
    Boid bigBird;
    std::cout << "boid pos: ( " << bigBird.getXPos() << ", " << bigBird.getYPos() << ", " << bigBird.getZPos() << " )" << std::endl;
    bigBird.setPos(1.0, 2.0, 3.0);
    std::cout << "boid new pos: ( " << bigBird.getXPos() << ", " << bigBird.getYPos() << ", " << bigBird.getZPos() << " )" << std::endl;
    // Test Flock
    std::cout << "\n" << "\n";
    Flock flock(10);
    flock.addBoid();
    flock.addBoid();
    flock.printBoid(8);
    flock.printBoid(9);
    flock.setNeighbours(6);
    flock.queryNeighbours(6);


    return 0;
}

