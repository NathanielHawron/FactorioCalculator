#pragma once

#include <vector>
#include <cstdint>

struct Recipe{
    struct Element{
        Material &mat;
        uint8_t amount = 1;
        float chance = 1.0f;
        uint8_t variation = 0;
    };
    private:
        static char labelBuffer[17];
        bool showList = false;
    public:
        Recipe();
        std::vector<Material> ingredients;
        std::vector<Material> results;
        char name[50];
        bool render(int index);
};