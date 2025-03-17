#include "imgui.h"

#include <string>

enum class MaterialType{
    ITEM,
    FLUID
};
struct Material{
    char name[30];
    void render(const char *label){
        ImGui::InputText(label, this->name,30);
    }
};