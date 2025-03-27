#pragma once

#include <vector>
#include <cstdint>

struct Machine{
    struct Tier{
        float speed = 1;
        float electricalConsumption = 0;
        float burnableConsumption = 0;
        float pollution = 0;
        uint32_t maxModules = 0;
        void render();
    };
    
    private:
        static char labelBuffer[17];
        static char tierBuffer[15];
        bool showList = false;

    public:
        std::size_t index;
        std::vector<Tier> tiers;
        char name[50];

        Machine(std::size_t index, const char *name = nullptr);
        bool render();
};