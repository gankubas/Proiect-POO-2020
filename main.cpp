#include <memory> // for std::shared_ptr
#include ".\include\workers.hpp" // also <iostream>, <string>, <vector>, <iterator>, <algorithm>, <fstream>, <sstream>, <map>, <utility>, <tuple>, <climits>

class AbstractWorker // abstract (almost) functor class
{
    protected:
        static workers::Elf buddy;
        static workers::Troll krampus;
        static workers::MsSanta jessica;

    public:
        virtual void doYourJob() = 0;
};

workers::Elf AbstractWorker::buddy;
workers::Troll AbstractWorker::krampus;
workers::MsSanta AbstractWorker::jessica;

class ConcreteElf : public AbstractWorker
{
    public:
        void doYourJob()
        {
            this->buddy.fillPresents();
            this->buddy.displayGifts();
        }
};

class ConcreteTroll : public AbstractWorker
{
    public:
        void doYourJob()
        {
            this->krampus.setGifts(this->buddy.getGifts());
            this->krampus.makePackages();
            this->krampus.displayPackages();
        }
};

class ConcreteSanta : public AbstractWorker
{
    protected:
        static workers::Santa kris;

    public:
        void doYourJob()
        {
            int start = 0;
            this->kris.optimizeRoad(0, 0, &start, "Rovaniemi");
            this->kris.displayShortestRoad();
        }
};

workers::Santa ConcreteSanta::kris(buddy.getCities());

class ConcreteMsSanta : public ConcreteSanta // needs to get route from a workers::Santa object
{
    public:
        void doYourJob()
        {
            this->jessica.setCandies(this->buddy.getCandies());
            this->jessica.setCoals(this->krampus.getUsedCoal());
            this->jessica.displayBudget();
            this->jessica.setPath(this->kris.getRoute());
        }
};

int main(void)
{
    // generate workforce
    std::vector<std::shared_ptr<AbstractWorker>> workforce;

    workforce.push_back(std::shared_ptr<AbstractWorker>(new ConcreteElf));
    workforce.push_back(std::shared_ptr<AbstractWorker>(new ConcreteTroll));
    workforce.push_back(std::shared_ptr<AbstractWorker>(new ConcreteSanta));
    workforce.push_back(std::shared_ptr<AbstractWorker>(new ConcreteMsSanta));

    try
    {
        for(std::shared_ptr<AbstractWorker> worker : workforce)
        {
            worker->doYourJob();
        }
    }
    catch(const char *err)
    {
        std::cout << err;

        return -1;
    }

    return 0;
}
