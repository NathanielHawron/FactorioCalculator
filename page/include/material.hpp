#pragma once
#include "imgui.h"

#include <string>

struct Material{
    private:
        static char matLabelBuffer[18];
        bool showList = false;
    public:
        std::size_t index;
        char name[50];
        float energy;
        Material(std::size_t index, const char *name = nullptr, float energy = 0);
        bool render();
};
