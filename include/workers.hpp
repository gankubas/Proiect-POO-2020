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

            ~Troll();

            std::vector<data::Present> getPackagedGifts() { return this->packaged_gifts; }
            std::map<std::string, int> getPackages() { return this->packages; }
            int getUsedCoal() { return this->used_coal; }

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

    Troll::~Troll()
    {
        this->packages.clear();
        this->gifts.clear();
        this->packaged_gifts.clear();
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
        private:
            static data::Toy candies;
            int num_of_candy;
            int num_of_coal;
            std::vector<data::Present> gifts;
            // TODO path variable from Santa class

            // only Ms Santa needs access to present modifier method
            void addCandies();

        public:
            ~MsSanta();

            void setCoals(int coals) { this->num_of_coal = coals; }
            void setCandies(int candies) { this->num_of_candy = candies; }
            void setGifts(std::vector<data::Present> new_gifts) { this->gifts = new_gifts; }

            double calculateExtraBudget();
    };

    data::Toy MsSanta::candies("candy", INT_MAX, 1.0);

    MsSanta::~MsSanta()
    {
        this->gifts.clear();
    }

    void MsSanta::addCandies()
    {
        for(data::Present gift : this->gifts)
        {
            gift.addToItems(this->candies);
        }
    }

    double MsSanta::calculateExtraBudget()
    {
        return this->num_of_coal * 0.5 + this->num_of_candy * 1.0;
    }

//-------------------------------Santa Class------------------------------------
    class Santa
    {

    };
}
