#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm> // for std::find_if

namespace data // used in case of overloaded functionality and for marking items from this header file
{
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
            double findItemCost(std::string item);

            friend std::istream &operator >>(std::istream &in, Toy &t); // used in populate.cpp by populateInventory()
            // seems to work even though it's declared inside a namespace?
                // TODO look up i/o operator overloading in custom namespaces

            friend class Present; // for adding names and prices
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

    double Toy::findItemCost(std::string item)
    {
        return item == this->name ? this->price : -1.0;
    }

    std::istream &operator >>(std::istream &in, Toy &t)
    {
        std::cout << "Name Amount Price:\n";

        in >> t.name >> t.amount >> t.price;

        return in;
    }

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

            void operator =(const Child &copy); // used by Child and Letter in ::populateLetters() in populate.cpp

            friend class DataComparator;
            friend class Letter; // operator == requires acces to protected attributes of another Child
                                    // unusre if child class has access to private members of parent-type objects
    };

    Child::Child(std::string new_name = "", std::string new_surname = "", std::string new_city = "", int new_age = -1)
    {
        this->name = new_name;
        this->surname = new_surname;
        this->city = new_city;
        this->age = new_age;
    }

    void Child::operator =(const Child &copy)
    {
        this->name = copy.name;
        this->surname = copy.surname;
        this->city = copy.city;
        this->age = copy.age;
    }

    class Letter : public Child
    {
        private:
            std::string colour;
            std::vector<std::string> wishlist;

        public:
            Letter(std::string new_name, std::string new_surname, std::string new_city, int new_age, std::string new_colour);
            Letter(const Child &copy);

            std::string getColour() { return this->colour; }
            std::vector<std::string> getWishlist() { return this->wishlist; }

            void setWishlist(std::vector<std::string> new_wishlist) { this->wishlist = new_wishlist; }
            void setColour(std::string new_colour) { this->colour = new_colour; }

            bool operator ==(const Child &c); // TODO remove if unused

            friend std::istream &operator >>(std::istream &in, Letter &l); // used in populate.cpp by populateLetters()
    };

    Letter::Letter(std::string new_name = "", std::string new_surname = "", std::string new_city = "", int new_age = -1, std::string new_colour = "")
        : Child{new_name, new_surname, new_city, new_age}
    {
        this->colour = new_colour;
    }

    Letter::Letter(const Child &copy)
    {
        this->name = copy.name;
        this->surname = copy.surname;
        this->city = copy.city;
        this->age = copy.age;
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

    class Present : public Child
    {
        private:
            std::string colour;
            std::vector<std::string> items;
            double cost;

        public:
            Present(std::string new_name, std::string new_surname, std::string new_city, int new_age);

            std::string getColour() { return this->colour; }
            std::vector<std::string> getItems() { return this->items; }
            double getCost() { return this->cost; }

            void setColour(std::string new_colour) { this->colour = new_colour; }

            void addToItems(Toy new_item);

            void operator =(const Present &copy); // used in workers::MsSanta::setGifts()
    };

    Present::Present(std::string new_name = "", std::string new_surname = "", std::string new_city = "", int new_age = -1)
        : Child{new_name, new_surname, new_city, new_age}
    {
        this->colour = "";
        this->cost = 0.0;
    }

    void Present::addToItems(Toy new_item)
    {
        this->items.push_back(new_item.name);
        this->cost += new_item.price;
    }

    void Present::operator =(const Present &copy)
    {
        this->name = copy.name;
        this->surname = copy.surname;
        this->city = copy.city;
        this->age = copy.age;
        this->colour = copy.colour;
        this->items = copy.items;
        this->cost = copy.cost;
    }

    class DataComparator // functor used by std::map when Child is key
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

//-----------------------------City & Road Classes------------------------------
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
