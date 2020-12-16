#include <map>
#include <utility> // for std::pair
#include <tuple>
#include <climits> // for INT_MAX
#include "data.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>

namespace workers // used in case of overloaded functionality and for marking items from this header file
{
//----------------------------------Elf Class-----------------------------------
    class Elf
    {

    };

//---------------------------------Troll Class----------------------------------
    class Troll
    {
        private:
            static data::Toy coals;
            static std::map<std::string, int> packages;
            static int used_coal;
            std::vector<std::tuple<data::Present, std::string, std::string>> gifts; // new struct/class?
            std::vector<data::Present> packaged_gifts;

            // only Troll objects require access to gift modifying methods
            void addCoals();
            void addPackaging();

        public:
            Troll();

            std::vector<data::Present> getPackagedGifts() { return this->packaged_gifts; }
            std::map<std::string, int> getPackages() { return this->packages; }

            void setGifts(std::vector<std::tuple<data::Present, std::string, std::string>> new_gifts) { this->gifts = new_gifts; }

            void makePackages();
    };

    data::Toy Troll::coals("coal", INT_MAX, 0.5);
    std::map<std::string, int> Troll::packages;
    int Troll::used_coal = 0;

    Troll::Troll()
    {
        this->packages["blue"] = 0;
        this->packages["pink"] = 0;
    }

    void Troll::addCoals()
    {
        for(std::tuple<data::Present, std::string, std::string> gift : this->gifts)
        {
            if(std::get<1>(gift) == "naughty")
            {
                std::get<0>(gift).addToItems(this->coals);
                this->used_coal++;
            }
        }
    }

    void Troll::addPackaging()
    {
        for(std::tuple<data::Present, std::string, std::string> gift : this->gifts)
        {
            std::get<0>(gift).setColour(std::get<2>(gift));

            if(std::get<2>(gift) == "blue")
            {
                this->packages["blue"]++;
            }
            else if(std::get<2>(gift) == "pink")
            {
                this->packages["pink"]++;
            }
        }
    }

    void Troll::makePackages()
    {
        this->addCoals();
        this->addPackaging();

        for(std::tuple<data::Present, std::string, std::string> gift : this->gifts)
        {
            this->packaged_gifts.push_back(std::get<0>(gift));
        }
    }

//------------------------------Ms Santa Class----------------------------------
    class MsSanta
    {

    };

//-------------------------------Santa Class------------------------------------
    class Santa
    {

    };
}
