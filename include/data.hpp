#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm> // for std::find_if

namespace data // used in case of overloaded functionality and for marking items from this header file
{
//---------------------------Child & Child by-products--------------------------
    class Child
    {
        protected:
            std::string name, surname, city;
            int age;

        public:
            Child(std::string new_name, std::string new_surname, std::string new_city, int new_age);

            std::string getName() { return this->name; }
            std::string getSurname() { return this->surname; }
            std::string getCity() { return this->city; }
            int getAge() { return this->age; }

            friend class DataComparator;
            friend class Letter; // operator == requires acces to protected attributes of Child
    };

    Child::Child(std::string new_name = "", std::string new_surname = "", std::string new_city = "", int new_age = -1)
    {
        this->name = new_name;
        this->surname = new_surname;
        this->city = new_city;
        this->age = new_age;
    }

    class Letter : public Child
    {
        private:
            std::string colour;
            std::vector<std::string> wishlist;

        public:
            Letter(std::string new_name, std::string new_surname, std::string new_city, int new_age, std::string new_colour);

            std::string getColour() { return this->colour; }
            std::vector<std::string> getWishlist() { return this->wishlist; }

            void addToWishlist(std::string new_item);

            bool operator ==(const Child &c);

            friend std::istream &operator >>(std::istream &in, Letter &l); // used in populate.cpp by populateLetters()

            friend class Present; // copy constructor requires acces to private attributes of Letter
    };

    Letter::Letter(std::string new_name = "", std::string new_surname = "", std::string new_city = "", int new_age = -1, std::string new_colour = "")
        : Child{new_name, new_surname, new_city, new_age}
    {
        this->colour = new_colour;
    }

    void Letter::addToWishlist(std::string new_item)
    {
        this->wishlist.push_back(new_item);
    }

    bool Letter::operator ==(const Child &c)
    {
        if(this->name == c.name
            && this->surname == c.surname
            && this->city == c.city
            && this->age == c.age) return true;
        else return false;
    }

    std::istream &operator >>(std::istream &in, Letter &l)
    {
        std::cout << "Name Surname City Age Colour Wishlist:\n";
        in >> l.name >> l.surname >> l.city >> l.colour;
        l.wishlist.clear();

        for(int i = 0; i < 50; i++)
        {
            std::string item;

            in >> item;

            if(item == "x") return in;
            else
            {
                l.wishlist.push_back(item);
            }
        }

        return in;
    }

    class Present : public Child // maybe redundant due to similarity with Letter
    {
        private:
            std::string colour;
            std::vector<std::string> items;

        public:
            Present(const Letter &l);

            std::string getColour() { return this->colour; }
            std::vector<std::string> getItems() { return this->items; }

            void addToItems(std::string new_item);
    };

    Present::Present(const Letter &l)
    {
        this->name = l.name;
        this->surname = l.surname;
        this->city = l.city;
        this->age = l.age;
        this->colour = l.colour;
    }

    void Present::addToItems(std::string new_item)
    {
        this->items.push_back(new_item);
    }

    class DataComparator // used by std::map when Child is key
    {
        public:
            bool operator ()(const Child &c1, const Child &c2)
            {
                if(c1.age < c2.age
                    || c1.city.compare(c2.city) < 0
                    || c1.surname.compare(c2.surname) < 0
                    || c1.name.compare(c2.name) < 0) return true;
                else return false;
            }
    };

//-------------------------------Inventory Items--------------------------------
    class Toy
    {
        private:
            std::string name;
            int amount;
            double price;

        public:
            Toy(std::string new_name, int new_amount, double new_price);

            std::string getName() { return this->name; }
            int getAmount() { return this->amount; }
            double getPrice() { return this->price; }

            void removeFromInventory();
            double findItem(std::string item);

            friend std::istream &operator >>(std::istream &in, Toy &t); // used in populate.cpp by populateInventory()
    };

    Toy::Toy(std::string new_name = "", int new_amount = -1, double new_price = -1.0)
    {
        this->name = new_name;
        this->amount = new_amount;
        this->price = new_price;
    }

    void Toy::removeFromInventory()
    {
        this->amount--;
    }

    double Toy::findItem(std::string item)
    {
        return item == this->name ? this->price : -1.0;
    }

    std::istream &operator >>(std::istream &in, Toy &t)
    {
        std::cout << "Name Amount Price:\n";

        in >> t.name >> t.amount >> t.price;

        return in;
    }

//----------------------------------City Class----------------------------------
    struct CityNode
    {
        public:
            std::string name;
            double distance;
            CityNode *path;

            CityNode(std::string new_name, double new_distance);
    };

    CityNode::CityNode(std::string new_name = "", double new_distance = 0)
    {
        this->name = new_name;
        this->distance = new_distance;
        this->path = nullptr;
    }

    class RoadGraph
    {
        private:
            static RoadGraph *start;
            RoadGraph(std::vector<std::string> new_start);

        public:
            int num_of_cities;
            std::vector<CityNode *> starting_city;
            std::vector<bool> isVisited;

            static RoadGraph *makeRoads(std::vector<std::string> new_start);

            void addPath(std::string start, std::string stop, double length);
    };

    RoadGraph *RoadGraph::start = nullptr;

    RoadGraph::RoadGraph(std::vector<std::string> new_start)
    {
        this->num_of_cities = new_start.size();

        for(int i = 0; i < new_start.size(); i++)
        {
            this->starting_city.push_back(new CityNode(new_start.at(i)));
        }
    }

    RoadGraph *RoadGraph::makeRoads(std::vector<std::string> new_start = {})
    {
        if(start == nullptr)
        {
            start = new RoadGraph(new_start);
        }

        return start;
    }

    void RoadGraph::addPath(std::string start, std::string stop, double length)
    {
        std::vector<CityNode *>::iterator start_pos = std::find_if(this->starting_city.begin(), this->starting_city.end(), [&start](const CityNode *starter)
                                                {
                                                    return starter->name == start;
                                                });

        CityNode *aux = this->starting_city.at(start_pos - this->starting_city.begin());
        while(aux->path != nullptr)
        {
            aux = aux->path;
        }

        aux->path = new CityNode(stop, length);
    }
}
