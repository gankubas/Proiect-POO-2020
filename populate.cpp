#include <iostream>
#include <fstream>
#include ".\include\ChildData.hpp"

void populateLetters()
{
    std::ofstream fout;
    std::ifstream fin;

    try
    {
        fout.open(".\\data\\Letters.dat", std::ofstream::binary);

        if(!fout.is_open())
        {
            throw "File cannot open.";
        }

        std::vector<Letter> children;
        Letter kid;

        for(int i = 0; i < 5; i++)
        {
            std::cin >> kid;

            children.push_back(kid);
        }

        fout.write(reinterpret_cast<char *>(&children), sizeof(children));

        fout.close();

        fin.open("Letters.dat", std::ifstream::binary);

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

int main()
{
    populateLetters();

    return 0;
}
