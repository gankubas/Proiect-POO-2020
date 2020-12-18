#include <fstream>
#include <sstream>
#include <cstdlib> // for std::srand(), std::rand()
#include <ctime> // for time(0) seed
#include ".\include\data.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>

template <typename type_t>
type_t getRandomElement(std::vector<type_t> collection)
{
    return collection.at(std::rand() % collection.size() + 1);
}

class RandomGenerator
{
    private:
        static RandomGenerator *rg;
        RandomGenerator();

    public:
        std::vector<std::string> inventory_gifts; // all possible gifts in inventory
        std::vector<double> inventory_prices; // prices for each possible gift
        std::vector<std::string> wishlist_gifts; // all possible gifts in wishlist
        std::vector<std::string> boy_names; // all possible male names
        std::vector<std::string> girl_names; // all possible female names
        std::vector<std::string> children_surnames; // all possible surnames
        std::vector<std::string> mexican_cities; // all possible city names
        std::vector<std::vector<double>> travelling_distances; // all inter-city distances
            // ^ matrice triunghiulara?

        static RandomGenerator *getGenerator();

        std::vector<data::Toy> getRInventory();
        std::vector<std::string> getRWishlist();
        data::RoadGraph getRCity();
        data::Child getRChild();
};

RandomGenerator *RandomGenerator::rg = nullptr;

RandomGenerator::RandomGenerator()
{
    std::ifstream fin;

    try
    {
        fin.open(".\\data\\Generator.dat");

        if(!fin.is_open())
        {
            throw "File cannot open.";
        }

        std::string aux;
        char to_variable = 'x';

        while(std::getline(fin, aux))
        {
            std::istringstream sin(aux);

            std::getline(sin, aux, ',');

            if(aux == "category")
            {
                std::getline(sin, aux);

                if(aux == "inventory") to_variable = 'i';
                else if(aux == "wishlist") to_variable = 'w';
                else if(aux == "boyname") to_variable = 'b';
                else if(aux == "girlname") to_variable = 'g';
                else if(aux == "surname") to_variable = 's';
                else if(aux == "city") to_variable = 'c';
                else throw "Unexpected data type.";
            }
            else
            {
                switch(to_variable)
                {
                    case 'i':
                    {
                        this->inventory_gifts.push_back(aux);
                        while(std::getline(sin, aux, ','))
                        {
                            this->inventory_gifts.push_back(aux);
                        }

                        std::getline(fin, aux);
                        std::istringstream temp(aux);

                        while(std::getline(temp, aux, ','))
                        {
                            this->inventory_prices.push_back(std::stod(aux));
                        }

                        break;
                    }

                    case 'w':
                    {
                        this->wishlist_gifts.push_back(aux);
                        while(std::getline(sin, aux, ','))
                        {
                            this->wishlist_gifts.push_back(aux);
                        }

                        break;
                    }

                    case 'b':
                    {
                        this->boy_names.push_back(aux);
                        while(std::getline(sin, aux, ','))
                        {
                            this->boy_names.push_back(aux);
                        }

                        break;
                    }

                    case 'g':
                    {
                        this->girl_names.push_back(aux);
                        while(std::getline(sin, aux, ','))
                        {
                            this->girl_names.push_back(aux);
                        }

                        break;
                    }

                    case 's':
                    {
                        this->children_surnames.push_back(aux);
                        while(std::getline(sin, aux, ','))
                        {
                            this->children_surnames.push_back(aux);
                        }

                        break;
                    }

                    case 'c':
                    {
                        this->mexican_cities.push_back(aux);
                        while(std::getline(sin, aux, ','))
                        {
                            this->mexican_cities.push_back(aux);
                        }

                        while(std::getline(fin, aux))
                        {
                            std::istringstream temp(aux);
                            std::vector<double> temp_vector;

                            while(std::getline(temp, aux, ','))
                            {
                                temp_vector.push_back(std::stod(aux));
                            }

                            this->travelling_distances.push_back(temp_vector);
                        }

                        break;
                    }

                    default:
                        throw "This shouldn't have happened.";
                }
            }
        }
    }
    catch(const char *err)
    {
        throw;
    }
}

RandomGenerator *RandomGenerator::getGenerator()
{
    if(rg == nullptr)
    {
        rg = new RandomGenerator;
    }

    return rg;
}

std::vector<data::Toy> RandomGenerator::getRInventory()
{
    std::vector<data::Toy> new_inventory;

    for(int i = 0; i < this->inventory_gifts.size(); i++)
    {
        if(std::rand() % 20) // 5% chance of item from database to be missing from inventory
        {
            // add 1-6 toys to inventory
            data::Toy temp(this->inventory_gifts.at(i), std::rand() % 6 + 1, this->inventory_prices.at(i));

            new_inventory.push_back(temp);
        }
    }

    return new_inventory;
}

std::vector<std::string> RandomGenerator::getRWishlist()
{
    std::vector<std::string> new_wishlist;

    for(std::string wish : this->wishlist_gifts)
    {
        if(!(std::rand() % 4)) // 25% chance of item to be in wishlist
        {
            new_wishlist.push_back(wish);
        }
    }

    return new_wishlist;
}

// data::RoadGraph RandomGenerator::getRCity()
// {
//     // TODO
// }

data::Child RandomGenerator::getRChild()
{
    std::string name;

    if(std::rand() % 2) // 50% chance of boy/girl
    {
        name = getRandomElement(this->boy_names);
    }
    else
    {
        name = getRandomElement(this->girl_names);
    }

    std::string surname = getRandomElement(this->children_surnames);
    std::string city = getRandomElement(this->mexican_cities); // maybe use cities after generation?
    int age = std::rand() % 10 + 4; // kids between 4-14 yo

    data::Child new_child(name, surname, city, age);

    return new_child;
}

std::vector<data::Child> populateLetters(RandomGenerator *gen)
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\Letters.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Letter> letters;
        std::vector<data::Child> children;

        for(int i = 0; i < std::rand() % 5 + 5; i++) // generate 5-10 letters
        {
            data::Child new_child = gen->getRChild();
            data::Letter new_letter(new_child);

            bool isBoy = false;
            for(std::string check_boy : gen->boy_names)
            {
                if(check_boy == new_child.getName())
                {
                    isBoy = true;
                    break;
                }
            }

            if(isBoy)
            {
                new_letter.setColour("blue");
            }
            else
            {
                new_letter.setColour("pink");
            }

            letters.push_back(new_letter);
            children.push_back(new_child);
        }

        fout.write(reinterpret_cast<char *>(&letters), sizeof(letters));

        fout.close();
        return children;
        // if(!fin.is_open())
        // {
        //     throw "File cannot open.";
        // }
        //
        // fin.read(reinterpret_cast<char *>(&outkids), sizeof(outkids));
        //
        // fin.close();
    }
    catch(const char *err)
    {
        throw;
    }
}

void populateInventory(RandomGenerator *gen)
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\Inventory.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Toy> inventory = gen->getRInventory();

        fout.write(reinterpret_cast<char *>(&inventory), sizeof(inventory));

        fout.close();
    }
    catch(const char *err)
    {
        throw;
    }
}

void populateList(RandomGenerator *gen)
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\List.dat");

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Child> children = ::populateLetters(gen);
        std::vector<std::string> naughtiness;

        for(int i = 0; i < children.size(); i++)
        {
            if(std::rand() % 10) // 10% of children are naughty
            {
                naughtiness.push_back("naughty");
            }
            else
            {
                naughtiness.push_back("nice");
            }
        }

        fout.write(reinterpret_cast<char *>(&children), sizeof(children));
        fout.write(reinterpret_cast<char *>(&naughtiness), sizeof(naughtiness));

        fout.close();
    }
    catch(const char *err)
    {
        throw;
    }
}

void populateCities(RandomGenerator *gen)
{
    // TODO
}

int main()
{
    std::srand(time(NULL));

    try
    {
        RandomGenerator *randgen = RandomGenerator::getGenerator();
        ::populateInventory(randgen);
        ::populateList(randgen); // also populateLetters()
        // ::populateCities(randgen);
    }
    catch(const char *err)
    {
        std::cout << err << "\nCheck files and try again.";

        return -1;
    }

    return 0;
}
