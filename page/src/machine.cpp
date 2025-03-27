#include "machine.hpp"

#include "imgui.h"
#include <iostream>

Machine::Machine(std::size_t index, const char* name):
index{index}{
    if(name != nullptr){
        memcpy(this->name, name, std::string(name).size()+1);
    }
}

bool Machine::render(){
    int temp = this->index;
    for(int i=5;i>=0;--i){
        int val = temp%16;
        temp /= 16;
        labelBuffer[i+10] = val <= 9 ? val+'0' : val+'A'-10;
    }
    ImGui::PushID(labelBuffer+10);
    ImGui::InputText(labelBuffer, this->name, 30);
    ImGui::SameLine();

    if(ImGui::Button(this->showList ? "H":"S")){
        this->showList = !this->showList;
    }
    ImGui::SameLine();
    bool res = ImGui::Button("X");
    if(this->showList){
        ImGui::BeginChild("Tiers",{700.0f,150.0f*this->tiers.size()+25.0f});
        if(ImGui::Button("Add Tier")){
            this->tiers.push_back({});
        }
        for(std::size_t i=0;i<this->tiers.size();++i){
            tierBuffer[12] = (i/10)+'0';
            tierBuffer[13] = (i%10)+'0';
            ImGui::PushID(tierBuffer+12);
            if(ImGui::Button(tierBuffer + ((i==0)*7)) && i>0){
                this->tiers.erase(this->tiers.begin() + i);
            }else{
                this->tiers.at(i).render();
            }
            ImGui::PopID();
        }


        ImGui::EndChild();
    }
    ImGui::PopID();
    return res;
}
void Machine::Tier::render(){
    ImGui::BeginChild("Tier",{650.0f,120.0f});
    ImGui::DragFloat("Speed",&this->speed,0.25f,0.25f,64,"%.2f");
    ImGui::DragFloat("Electrical Consumption",&this->electricalConsumption,1,1/256,256,"%.3f");
    ImGui::DragFloat("Burnable Consumption",&this->burnableConsumption,1,1/256,256,"%.3f");
    ImGui::DragFloat("Pollution",&this->pollution,1,1/256,256,"%.3f");
    ImGui::DragScalar("Max Modules",ImGuiDataType_U8,&this->maxModules);
    ImGui::EndChild();
}

char Machine::labelBuffer[] = "Machine 0x______";
char Machine::tierBuffer[] = "Remove Tier __";