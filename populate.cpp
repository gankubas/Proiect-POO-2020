#include <fstream>
#include <sstream>
#include <random>
#include <chrono> // for time-based seed
#include ".\include\data.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>

template <typename type_t>
type_t getRandomElement(std::vector<type_t> collection, std::default_random_engine gen) // returns a random element from a vector
{
    std::uniform_int_distribution<int> random(0, collection.size() - 1);

    return collection.at(random(gen));
}

class RandomGenerator
{
    private:
        static RandomGenerator *rg;

        std::vector<std::string> inventory_gifts; // all possible gifts in inventory
        std::vector<double> inventory_prices; // prices for each possible gift
        std::vector<std::string> wishlist_gifts; // all possible gifts in wishlist
        std::vector<std::string> girl_names; // all possible female names
        std::vector<std::string> children_surnames; // all possible surnames
        std::vector<std::string> mexican_cities; // all possible city names
        std::vector<std::vector<double>> travelling_distances; // all inter-city distances
            // ^ matrice triunghiulara?

        void getRCity(); // only RandomGenerator should modify the RoadGraph

        RandomGenerator();

    public:
        static std::default_random_engine rng; // same seeded engine during the same run
        data::RoadGraph *generated_roadgraph;
        std::vector<data::Child> generated_children;

        std::vector<std::string> boy_names; // all possible male names
                                                // public because ::populateLetters() requires access

        static RandomGenerator *getGenerator();

        std::vector<data::Toy> getRInventory();
        std::vector<std::string> getRWishlist();
        std::vector<data::Child> getRChild();
};

RandomGenerator *RandomGenerator::rg = nullptr;

// random engine using current time as seed
std::default_random_engine RandomGenerator::rng (std::chrono::system_clock::now().time_since_epoch().count());

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
        rg->getRCity();
    }

    return rg;
}

std::vector<data::Toy> RandomGenerator::getRInventory()
{
    std::vector<data::Toy> new_inventory;
    std::uniform_int_distribution<int> random1(0, 19);
    std::uniform_int_distribution<int> random2(1, 6);

    for(int i = 0; i < this->inventory_gifts.size(); i++)
    {
        if(random1(this->rng)) // 5% chance of item from database to be missing from inventory
        {
            // add 1-6 toys to inventory
            data::Toy temp(this->inventory_gifts.at(i), random2(this->rng), this->inventory_prices.at(i));

            new_inventory.push_back(temp);
        }
    }

    return new_inventory;
}

std::vector<std::string> RandomGenerator::getRWishlist()
{
    std::vector<std::string> new_wishlist;
    std::uniform_int_distribution<int> random(0, 3);

    for(std::string wish : this->wishlist_gifts)
    {
        if(!random(this->rng)) // 25% chance of item to be in wishlist
        {
            new_wishlist.push_back(wish);
        }
    }

    // shuffle the wishlist because first wishes have priority when presents are made
    std::random_shuffle(new_wishlist.begin(), new_wishlist.end());

    return new_wishlist;
}

void RandomGenerator::getRCity()
{
    std::vector<std::string> city_names;
    std::vector<int> positions;
    int count = 0;
    std::uniform_int_distribution<int> random(0, 2);

    while(count < 4) // at least 4 cities
    {
        for(int i = 0; i < this->mexican_cities.size(); i++)
        {
            // make sure no duplicate cities are added
            bool isSeen = std::find(positions.begin(), positions.end(), i) != positions.end();
            if(!random(this->rng) && !isSeen) // 33% chance of city being in the final list
            {
                city_names.push_back(this->mexican_cities.at(i));
                positions.push_back(i);

                count++;
            }
        }
    }

    std::vector<std::vector<double>> city_distances(count + 1, std::vector<double>(count + 1));
    for(int i = 0; i < count; i++)
    {
        for(int j = 0; j < count; j++)
        {
            city_distances.at(i).at(j) = this->travelling_distances.at(positions.at(i)).at(positions.at(j));
        }
    }

    city_names.push_back("Rovaniemi");
    for(int i = 0; i < count; i++)
    {
        city_distances.at(count).at(i) = this->travelling_distances.at(this->mexican_cities.size()).at(positions.at(i));
    }

    this->generated_roadgraph = data::RoadGraph::makeRoads(city_names);

    for(int i = 0; i <= count; i++)
    {
        for(int j = 0; j < count; j++)
        {
            this->generated_roadgraph->addPath(city_names.at(i), city_names.at(j), city_distances.at(i).at(j));
        }

        if(i != count)
        {
            this->generated_roadgraph->addPath(city_names.at(i), "Rovaniemi", city_distances.at(count).at(i));
        }
    }
}

std::vector<data::Child> RandomGenerator::getRChild()
{
    if(this->generated_children.empty())
    {
        std::vector<std::string> visited_cities;
        std::vector<std::string> cities_to_visit; // no children in starting city
        std::uniform_int_distribution<int> random1(0, 1);
        std::uniform_int_distribution<int> random2(4, 14);

        for(data::CityNode *i : this->generated_roadgraph->starting_city)
        {
            if(i->name != "Rovaniemi")
            {
                cities_to_visit.push_back(i->name);
            }
        }

        while(visited_cities.size() != cities_to_visit.size()) // at least 1 child/city
        {
            for(int i = 0; i < 5; i++) // otherwise last city to be added is guaranteed to only have 1 child
                                            // also generates the minimum of 5 children regardless of the number of cities
            {
                std::string name;

                if(random1(this->rng)) // 50% chance of boy/girl
                {
                    name = getRandomElement(this->boy_names, this->rng);
                }
                else
                {
                    name = getRandomElement(this->girl_names, this->rng);
                }

                std::string surname = getRandomElement(this->children_surnames, this->rng);
                std::string city = getRandomElement(cities_to_visit, this->rng);

                if(std::find(visited_cities.begin(), visited_cities.end(), city) == visited_cities.end())
                {
                    visited_cities.push_back(city);
                }

                int age = random2(this->rng); // kids between 4-14 y.o.

                this->generated_children.push_back(data::Child(name, surname, city, age));
            }
        }
    }

    return this->generated_children;
}

void populateInventory(RandomGenerator *gen)
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\Inventory.dat");

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Toy> inventory = gen->getRInventory();

        for(data::Toy item : inventory)
        {
            fout << item;
        }

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
        fout.open(".\\data\\List.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Child> children = gen->getRChild();
        std::vector<std::string> naughtiness;
        std::uniform_int_distribution<int> random(0, 9);

        for(int i = 0; i < children.size(); i++)
        {
            if(!random(gen->rng)) // 10% of children are naughty
            {
                naughtiness.push_back("naughty");
            }
            else
            {
                naughtiness.push_back("nice");
            }
        }

        for(int i = 0; i < children.size(); i++)
        {
            fout << children.at(i) << "," << naughtiness.at(i) << "\n";
        }

        fout.close();
    }
    catch(const char *err)
    {
        throw;
    }
}

void populateCities(RandomGenerator *gen)
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\Cities.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        data::RoadGraph *roads = gen->generated_roadgraph;

        fout << *roads;

        fout.close();
    }
    catch(const char *err)
    {
        throw;
    }
}

void populateLetters(RandomGenerator *gen)
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
        std::vector<data::Child> children = gen->getRChild();

        for(data::Child new_child : children)
        {
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

            new_letter.setWishlist(gen->getRWishlist());

            letters.push_back(new_letter);
        }

        for(data::Letter i : letters)
        {
            fout << i;
        }

        fout.close();
    }
    catch(const char *err)
    {
        throw;
    }
}

int main()
{
    try
    {
        RandomGenerator *randgen = RandomGenerator::getGenerator();
        ::populateInventory(randgen);
        ::populateList(randgen);
        ::populateCities(randgen);
        ::populateLetters(randgen);
    }
    catch(const char *err)
    {
        std::cout << err << "\nCheck files and try again.";

        return -1;
    }

    return 0;
}
