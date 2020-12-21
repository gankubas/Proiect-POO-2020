#include <fstream>
#include <sstream>
#include ".\include\workers.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>, <map>, <utility>, <tuple>, <climits>

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

int main(void)
{
    SingletonDatabase *LaponianDatabase = SingletonDatabase::getDatabase();

    return 0;
}

// TODO
/*
4 cities/country includes finland?
MsSanta prints total sum or extra sum?
*/

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
