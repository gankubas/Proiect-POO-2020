#include ".\include\workers.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>, <fstream>, <sstream>, <map>, <utility>, <tuple>, <climits>

int main(void)
{
    workers::Elf buddy;
    workers::Troll krampus;
    workers::MsSanta jessica;
    workers::Santa kris;

    try
    {
        buddy.fillPresents();
        buddy.displayGifts();

        krampus.setGifts(buddy.getGifts());
        krampus.makePackages();
        krampus.displayPackages();

        jessica.setCandies(buddy.getCandies());
        jessica.setCoals(krampus.getUsedCoal());
        jessica.displayBudget();

        kris.setPath(buddy.getCities());

        jessica.setPath(kris.getRoute());
    }
    catch(const char *err)
    {
        std::cout << err;

        return -1;
    }

    return 0;
}

// TODO
/*-----------------------Bullet point checklist---------------------------------
------------------------------Item: lines---------------------------------------
Class:
Object:
Constructor:
Destructor:
Encapsulation:
Inheritance:
Polymorphism:
Abstraction:
Template:
Exception:
*/
