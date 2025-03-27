#include "main.hpp"

template <class T>
void renderDeletableVector(std::vector<T> &vec){
    for(auto it = vec.begin(); it != vec.end(); ++it){
        if(it->render()){
            it = vec.erase(it);
            auto iter = it;
            while(iter != vec.end()){
                --iter->index;
                ++iter;
            }
        }
    }
}

void materialsMenu(State &state){
    auto &materials = state.materials;
    ImGui::BeginChild("MaterialList",{750,500});
    renderDeletableVector(materials);
    ImGui::EndChild();
    if(ImGui::Button("Add Material")){
        materials.push_back({materials.size(),"mat"});
    }
}

void machinesMenu(State &state){
    auto &machines = state.machines;
    ImGui::BeginChild("MachineList",{750,500});
    renderDeletableVector(machines);
    ImGui::EndChild();
    if(ImGui::Button("Add Machine")){
        machines.push_back({machines.size(),"mac"});
        (machines.end()-1)->tiers.push_back({});
    }
}