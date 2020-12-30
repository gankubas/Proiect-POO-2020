#include <fstream>
#include <sstream>
#include <map>
#include <utility> // for std::pair
#include <tuple>
#include <climits> // for INT_MAX
#include <iomanip> // for double-decimal printing
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
            template <class class_t> // used with data::Present and data::Letter to find naughtiness of child
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
                std::vector<std::string> gift_wish = i.getWishlist();

                if(child_in_list->second == "naughty")
                {
                    budget = 10.00;
                }
                else if(child_in_list->second == "nice")
                {
                    budget = 100.00;
                }

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

            // could be implemented in Elf::getChildFromList() ?
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
            data::RoadGraph *travel_data; // all cities to vosot and distances between them
            data::CityNode *optimal_route; // final route to send to workers::MsSanta
            double min_length; // shortest path
            std::vector<bool> isVisited; // used for DFT traversal in optimizeRoad(...)
            std::vector<std::vector<std::string>> paths; // remembers all possible paths
            int best_poz; // remembers which path is the best
            std::vector<std::string> temp; // used to remember current check in optimizeRoad(...)

        public:
            Santa(data::RoadGraph *roads);

            data::CityNode *getRoute();

            void optimizeRoad(int visits, double cur_length, int *poz, std::string last);
            void displayShortestRoad();
    };

    Santa::Santa(data::RoadGraph *roads)
    {
        this->travel_data = roads;
        this->min_length = 0.0;
        this->best_poz = -1;

        for(int i = 1; i <= this->travel_data->starting_city.size(); i++)
        {
            this->isVisited.push_back(false);
        }
    }

    data::CityNode *Santa::getRoute()
    {
        data::CityNode *final_destination = new data::CityNode("Rovaniemi");
        data::CityNode *aux = final_destination;

        for(int i = 0; i < this->paths.at(this->best_poz).size(); i++)
        {
            std::string name = this->paths.at(this->best_poz).at(i);
            aux->path = new data::CityNode(name, this->travel_data->getDistance(aux->name, name));

            aux = aux->path;
        }

        return final_destination;
    }

    void Santa::optimizeRoad(int visits, double cur_length, int *poz, std::string last)
    {
        // exit recursive loop
        if(visits == this->isVisited.size() - 1)
        {
            // get new minimum length
            if(this->min_length == 0.0)
            {
                this->min_length = cur_length;
                this->best_poz = *poz;
            }
            else
            {
                this->min_length = std::min(this->min_length, cur_length);

                if(this->min_length == cur_length)
                {
                    this->best_poz = *poz;
                }
            }

            // prepare for next permutation
            (*poz)++;
            this->paths.push_back(temp);
            return;
        }

        for(int i = 0; i < this->travel_data->starting_city.size() - 1; i++)
        {
            if(!isVisited.at(i))
            {
                // visit current node
                std::string next = this->travel_data->starting_city.at(i)->name;
                isVisited.at(i) = true;
                temp.push_back(next);

                // continue path
                // incrementing in function call removes the need to deincrement after the call
                this->optimizeRoad(visits + 1, cur_length + this->travel_data->getDistance(last, next), poz, next);

                // forget visit for new paths
                temp.pop_back();
                isVisited.at(i) = false;
            }
        }
    }

    void Santa::displayShortestRoad()
    {
        std::cout << "Rovaniemi";

        for(int i = 0; i < this->paths.at(this->best_poz).size(); i++)
        {
            std::cout << " -> " << this->paths.at(this->best_poz).at(i);
        }

        std::cout << std::fixed << std::setprecision(2) << "\nLength: " << this->min_length << "km\n";
    }
}
