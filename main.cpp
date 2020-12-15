#include <iostream>
#include ".\include\ChildData.hpp"
#include <iterator>
#include <map>
#include <utility>

class SingletonDatabase
{
    private:
        static SingletonDatabase *sdb;
        SingletonDatabase();

    public:
        std::map<Child, std::string, DataComparator> naughty_nice;
        std::vector<Letter> recieved_mail;

        static SingletonDatabase *getDatabase();
};

int main(void)
{


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
