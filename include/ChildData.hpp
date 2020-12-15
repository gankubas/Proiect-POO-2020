#include <iostream>
#include <string>
#include <vector>

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
        friend class Letter;
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
        void addToWishlist(std::string new_item);

        bool operator ==(const Child &c);

        friend std::istream &operator >>(std::istream &in, Letter &l);

        friend class Present;
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

class DataComparator
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
