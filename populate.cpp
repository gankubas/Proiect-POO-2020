#include <fstream>
#include ".\include\data.hpp"

void populateLetters()
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\Letters.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Letter> children;
        data::Letter kid;

        for(int i = 0; i < 50; i++)
        {
            std::cin >> kid;

            children.push_back(kid);
        }

        fout.write(reinterpret_cast<char *>(&children), sizeof(children));

        fout.close();

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
        std::cout << err;
    }
}

void populateInventory()
{
    std::ofstream fout;

    try
    {
        fout.open(".\\data\\Inventory.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<data::Toy> inventory;
        data::Toy item;

        for(int i = 0; i < 50; i++)
        {
            std::cin >> item;

            inventory.push_back(item);
        }

        fout.write(reinterpret_cast<char *>(&inventory), sizeof(inventory));

        fout.close();
    }
    catch(const char *err)
    {
        std::cout << err;
    }
}

void populateCities()
{

}

int main()
{
    populateLetters();
    populateInventory();
    populateCities();

    return 0;
}
