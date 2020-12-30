#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm> // for std::find_if, std::random_shuffle (in populate.cpp) and std::min(in workers.hpp)

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

            friend class Present; // for adding names and prices

            friend std::ostream &operator <<(std::ostream &out, const Toy &t);
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

    std::ostream &operator <<(std::ostream &out, const Toy &t)
    {
        out << t.name << "," << t.amount << "," << t.price << "\n";

        return out;
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
            friend class Letter; // copy constructor & operator == require access to protected attributes of another Child
            friend class Present; // copy constructor requires access to protected attributes of another Child

            friend std::ostream &operator <<(std::ostream &out, const Child &c);
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

    std::ostream &operator <<(std::ostream &out, const Child &c)
    {
        out << c.name << "," << c.surname << "," << c.city << "," << c.age;

        return out;
    }

    class Letter : public Child
    {
        private:
            std::string colour;
            std::vector<std::string> wishlist;

        public:
            Letter(std::string new_name, std::string new_surname, std::string new_city, int new_age, std::string new_colour);
            Letter(const Child &copy);

            ~Letter();

            std::string getColour() { return this->colour; }
            std::vector<std::string> getWishlist() { return this->wishlist; }

            void setWishlist(std::vector<std::string> new_wishlist) { this->wishlist = new_wishlist; }
            void setColour(std::string new_colour) { this->colour = new_colour; }

            bool operator ==(const Child &c);

            friend class Present; // operator == requires access to protected attributes of another Letter

            friend std::ostream &operator <<(std::ostream &out, const Letter &l);
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
        this->colour = "";
        this->wishlist = {};
    }

    Letter::~Letter()
    {
        this->wishlist.clear();
    }

    bool Letter::operator ==(const Child &c)
    {
        if(this->name == c.name
            && this->surname == c.surname
            && this->city == c.city
            && this->age == c.age) return true;
        else return false;
    }

    std::ostream &operator <<(std::ostream &out, const Letter &l)
    {
        out << l.name << "," << l.surname << "," << l.city << "," << l.age << "," << l.colour << ",";

        for(int i = 0; i < l.wishlist.size() - 1; i++)
        {
            out << l.wishlist.at(i) << ",";
        }
        out << l.wishlist.at(l.wishlist.size() - 1) << "\n";

        return out;
    }

    class Present : public Child
    {
        private:
            std::string colour;
            std::vector<std::string> items;
            double cost;

        public:
            Present(std::string new_name, std::string new_surname, std::string new_city, int new_age);
            Present(const Child &copy);

            ~Present();

            std::string getColour() { return this->colour; }
            std::vector<std::string> getItems() { return this->items; }
            double getCost() { return this->cost; }

            void setColour(std::string new_colour) { this->colour = new_colour; }

            void addToItems(Toy new_item);

            void operator =(const Present &copy); // used in workers::MsSanta::setGifts()
            bool operator ==(const Letter &l);

            friend std::ostream &operator <<(std::ostream &out, const Present &p);
    };

    Present::Present(std::string new_name = "", std::string new_surname = "", std::string new_city = "", int new_age = -1)
        : Child{new_name, new_surname, new_city, new_age}
    {
        this->colour = "";
        this->cost = 0.0;
    }

    Present::Present(const Child &copy)
    {
        this->name = copy.name;
        this->surname = copy.surname;
        this->city = copy.city;
        this->age = copy.age;
        this->colour = "";
        this->cost = 0.0;
    }

    Present::~Present()
    {
        this->items.clear();
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

    bool Present::operator ==(const Letter &l)
    {
        if(this->name == l.name
            && this->surname == l.surname
            && this->city == l.city
            && this->age == l.age) return true;
        else return false;
    }

    std::ostream &operator <<(std::ostream &out, const Present &p)
    {
        out << "\n" << p.name << " " << p.surname << ", " << p.age << ", from " << p.city << " has received:\n";

        for(Toy i : p.items)
        {
            out << i.getName() << "\n";
        }

        return out;
    }

    class DataComparator // functor used by std::map with Child as key
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

            ~CityNode();
    };

    CityNode::CityNode(std::string new_name = "", double new_distance = 0.0)
    {
        this->name = new_name;
        this->distance = new_distance;
        this->path = nullptr;
    }

    CityNode::~CityNode()
    {
        delete this->path;
    }

    class RoadGraph
    {
        private:
            static RoadGraph *start;
            RoadGraph(std::vector<std::string> new_start);

        public:
            std::vector<CityNode *> starting_city;

            static RoadGraph *makeRoads(std::vector<std::string> new_start);

            void addPath(std::string start, std::string stop, double length);
            double getDistance(std::string start, std::string stop);

            friend std::ostream &operator <<(std::ostream &out, const RoadGraph &r);
    };

    RoadGraph *RoadGraph::start = nullptr;

    RoadGraph::RoadGraph(std::vector<std::string> new_start)
    {
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
        std::vector<CityNode *>::iterator start_pos = std::find_if(this->starting_city.begin(), this->starting_city.end(),
                                                        [&start](const CityNode *starter)
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

    double RoadGraph::getDistance(std::string start, std::string stop)
    {
        std::vector<CityNode *>::iterator start_pos = std::find_if(this->starting_city.begin(), this->starting_city.end(),
                                                        [&start](const CityNode *starter)
                                                        {
                                                            return starter->name == start;
                                                        });

        CityNode *aux = this->starting_city.at(start_pos - this->starting_city.begin());
        while(aux->name != stop)
        {
            aux = aux->path;
        }

        return aux->distance;
    }

    std::ostream &operator <<(std::ostream &out, const RoadGraph &r)
    {
        for(int i = 0; i < r.starting_city.size() - 1; i++)
        {
            out << r.starting_city.at(i)->name << ",";
        }
        out << r.starting_city.at(r.starting_city.size() - 1)->name << "\n";

        for(int i = 0; i < r.starting_city.size(); i++)
        {
            CityNode *c = r.starting_city.at(i)->path;

            while(c->path != nullptr)
            {
                out << c->distance << ",";
                c = c->path;
            }

            out << c->distance << "\n";
        }

        return out;
    }
}
