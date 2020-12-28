#include <fstream>
#include <sstream>
#include <map>
#include <utility> // for std::pair
#include <tuple>
#include <climits> // for INT_MAX
#include "data.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>

namespace workers // used in case of overloaded functionality and for marking items from this header file
{
//------------------------------Database Class----------------------------------
    class SingletonDatabase
    {
        private:
            static SingletonDatabase *sdb;
            SingletonDatabase();

        public:
            std::vector<data::Toy> current_inventory;
            std::map<data::Child, std::string, data::DataComparator> naughty_nice;
            data::RoadGraph *travelling_paths;
            std::vector<data::Letter> received_mail;

            static SingletonDatabase *getDatabase();
    };

    SingletonDatabase *SingletonDatabase::sdb = nullptr;

    SingletonDatabase::SingletonDatabase()
    {
        std::ifstream fin;
        std::string aux;

        try
        {
            fin.open(".\\data\\Inventory.dat");

            if(!fin.is_open())
            {
                throw "Read failed.";
            }

            while(std::getline(fin, aux))
            {
                std::istringstream sin(aux);

                std::getline(sin, aux, ',');
                std::string name = aux;

                std::getline(sin, aux, ',');
                int amount = std::stoi(aux);

                std::getline(sin, aux);
                double price = std::stod(aux);

                this->current_inventory.push_back(data::Toy(name, amount, price));
            }

            fin.close();

            fin.open(".\\data\\List.dat");

            if(!fin.is_open())
            {
                throw "Read failed.";
            }

            while(std::getline(fin, aux))
            {
                std::istringstream sin(aux);

                std::getline(sin, aux, ',');
                std::string name = aux;

                std::getline(sin, aux, ',');
                std::string surname = aux;

                std::getline(sin, aux, ',');
                std::string city = aux;

                std::getline(sin, aux, ',');
                int age = std::stoi(aux);

                std::getline(sin, aux);

                this->naughty_nice.insert(std::pair<data::Child, std::string>(data::Child(name, surname, city, age), aux));
            }

            fin.close();

            fin.open(".\\data\\Cities.dat");

            if(!fin.is_open())
            {
                throw "Read failed.";
            }

            std::vector<std::string> city_names;

            std::getline(fin, aux);
            std::istringstream sin(aux);

            while(std::getline(sin, aux, ','))
            {
                city_names.push_back(aux);
            }

            int i = 0, j = 0;
            std::vector<std::vector<double>> city_distances(city_names.size(), std::vector<double>(city_names.size()));

            while(std::getline(fin, aux))
            {
                std::istringstream tempin(aux);

                j = 0;
                while(std::getline(tempin, aux, ','))
                {
                    city_distances.at(i).at(j) = std::stod(aux);
                    j++;
                }
                i++;
            }

            this->travelling_paths = data::RoadGraph::makeRoads(city_names);

            for(i = 0; i < city_names.size(); i++)
            {
                for(j = 0; j < city_names.size() - 1; j++)
                {
                    this->travelling_paths->addPath(city_names.at(i), city_names.at(j), city_distances.at(i).at(j));
                }
            }

            fin.close();

            fin.open(".\\data\\Letters.dat");

            if(!fin.is_open())
            {
                throw "Read failed.";
            }

            while(std::getline(fin, aux))
            {
                std::istringstream sin(aux);

                std::getline(sin, aux, ',');
                std::string name = aux;

                std::getline(sin, aux, ',');
                std::string surname = aux;

                std::getline(sin, aux, ',');
                std::string city = aux;

                std::getline(sin, aux, ',');
                int age = std::stoi(aux);

                std::getline(sin, aux, ',');
                std::string colour = aux;

                data::Letter mail(name, surname, city, age, colour);
                std::vector<std::string> gifts;

                while(std::getline(sin, aux, ','))
                {
                    gifts.push_back(aux);
                }

                mail.setWishlist(gifts);

                this->received_mail.push_back(mail);
            }

            fin.close();
        }
        catch(const char *err)
        {
            throw;
        }
    }

    SingletonDatabase *SingletonDatabase::getDatabase()
    {
        if(sdb == nullptr)
        {
            sdb = new SingletonDatabase;
        }

        return sdb;
    }

//----------------------------------Elf Class-----------------------------------
    class Elf
    {
        private:
            static SingletonDatabase *fulldb;
            static data::Toy default_naughty, default_nice;
            static int candy_count;
            std::vector<data::Present> gifts;

        public:
            template <class class_t>
            std::map<data::Child, std::string>::iterator getChildFromList(class_t item);

            void fillPresents();

            std::vector<std::tuple<data::Present, std::string, std::string>> getGifts();
            int getCandies() { return this->candy_count; }
            data::RoadGraph *getCities() { return this->fulldb->travelling_paths; }

            void displayGifts();
    };

    SingletonDatabase *Elf::fulldb = SingletonDatabase::getDatabase();
    data::Toy Elf::default_naughty("Socks", INT_MAX, 10.00);
    data::Toy Elf::default_nice("Sleigh", INT_MAX, 100.00);
    int Elf::candy_count = 0;

    template <class class_t>
    std::map<data::Child, std::string>::iterator Elf::getChildFromList(class_t item)
    {
        std::map<data::Child, std::string>::iterator child_from_list;

        child_from_list = std::find_if(this->fulldb->naughty_nice.begin(), this->fulldb->naughty_nice.end(),
                            [&item](const std::pair<data::Child, std::string> mail)
                            {
                                return item == mail.first;
                            });

        if(child_from_list == this->fulldb->naughty_nice.end())
        {
            throw "Child not found.";
        }

        return child_from_list;
    }

    void Elf::fillPresents()
    {
        for(data::Letter i : this->fulldb->received_mail)
        {
            try
            {
                double budget, cost = 0.00;
                std::map<data::Child, std::string>::iterator child_in_list = this->getChildFromList(i);

                if(child_in_list->second == "naughty")
                {
                    budget = 10.00;
                }
                else if(child_in_list->second == "nice")
                {
                    budget = 100.00;
                }

                std::vector<std::string> gift_wish = i.getWishlist();
                data::Present new_gift(child_in_list->first);

                for(std::string j : gift_wish)
                {
                    for(data::Toy k : this->fulldb->current_inventory)
                    {
                        if(j == k.getName()
                            && budget > k.getPrice()
                            && k.getAmount() > 0
                            && (cost + k.getPrice()) <= budget)
                        {
                            new_gift.addToItems(k);
                            cost += k.getPrice();
                            k.removeFromInventory();
                        }
                    }
                }

                if(cost == 0)
                {
                    if(budget == 10)
                    {
                        new_gift.addToItems(this->default_naughty);
                    }
                    else if(budget == 100)
                    {
                        new_gift.addToItems(this->default_nice);
                    }
                }

                this->gifts.push_back(new_gift);
                this->candy_count += (int)(budget - cost);
            }
            catch(const char *err)
            {
                throw;
            }
        }
    }

    std::vector<std::tuple<data::Present, std::string, std::string>> Elf::getGifts()
    {
        std::vector<std::tuple<data::Present, std::string, std::string>> gift_info;

        for(data::Present i : this->gifts)
        {
            std::map<data::Child, std::string>::iterator child_in_list = this->getChildFromList(i);
            std::vector<data::Letter>::iterator child_from_mail;
            std::string status, gender;

            child_from_mail = std::find_if(this->fulldb->received_mail.begin(), this->fulldb->received_mail.end(),
                                [&i](const data::Letter mail)
                                {
                                    return i == mail;
                                });

            if(child_from_mail == this->fulldb->received_mail.end())
            {
                throw "Child not found.";
            }

            status = child_in_list->second;
            gender = child_from_mail->getColour();

            std::tuple<data::Present, std::string, std::string> new_info(i, status, gender);
            gift_info.push_back(new_info);
        }

        return gift_info;
    }

    void Elf::displayGifts()
    {
        for(data::Present i : this->gifts)
        {
            std::cout << i;
        }
    }

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
            void displayPackages();
    };

    data::Toy Troll::coals("coal", INT_MAX, 0.50);
    std::map<std::string, int> Troll::packages = {};
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

    void Troll::displayPackages()
    {
        std::cout << "\nThe trolls used " << this->packages["blue"] << " blue packages and "
                    << this->packages["pink"] << " pink packages.\n";
    }

//------------------------------Ms Santa Class----------------------------------
    class MsSanta
    {
        private:
            int num_of_candy;
            int num_of_coal;
            std::vector<data::Present> gifts;
            data::CityNode *final_path;

        public:
            ~MsSanta();

            void setCoals(int coals) { this->num_of_coal = coals; }
            void setCandies(int candies) { this->num_of_candy = candies; }
            void setGifts(std::vector<data::Present> new_gifts) { this->gifts = new_gifts; }
            void setPath(data::CityNode *new_path) { this->final_path = new_path; }

            double calculateExtraBudget();
            void displayBudget();
    };

    MsSanta::~MsSanta()
    {
        this->gifts.clear();
    }

    double MsSanta::calculateExtraBudget()
    {
        return this->num_of_coal * 0.5 + this->num_of_candy * 1.0;
    }

    void MsSanta::displayBudget()
    {
        std::cout << "An extra $" << this->calculateExtraBudget() << " has been used.\n";
    }

//-------------------------------Santa Class------------------------------------
    class Santa
    {
        private:
            data::RoadGraph *travel_data;
            data::CityNode *route;

        public:
            data::CityNode *getRoute() { return this->route; }

            void setPath(data::RoadGraph *roads) { this->travel_data = roads; }
    };
}
