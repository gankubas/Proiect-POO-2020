#include <iterator>
#include <map>
#include <utility>
#include ".\include\data.hpp"


class SingletonDatabase
{
    private:
        static SingletonDatabase *sdb;
        SingletonDatabase();

    public:
        std::map<data::Child, std::string, data::DataComparator> naughty_nice;
        std::vector<data::Letter> recieved_mail;

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
