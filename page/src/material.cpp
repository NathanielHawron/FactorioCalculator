#include "material.hpp"

Material::Material(std::size_t index, const char* name, float energy):
index{index},
energy{energy}{
    if(name != nullptr){
        memcpy(this->name, name, std::string(name).size()+1);
    }
}

bool Material::render(){
    std::size_t temp = this->index;
    for(int i=16;i>=11;--i){
        int val = temp%16;
        temp /= 16;
        matLabelBuffer[i] = val <= 9 ? val+'0' : val+'A'-10;
    }
    ImGui::PushID(matLabelBuffer+11);
    ImGui::InputText(matLabelBuffer, this->name, 30);
    ImGui::SameLine();

    if(ImGui::Button(this->showList ? "H":"S")){
        this->showList = !this->showList;
    }
    ImGui::SameLine();
    int res = ImGui::Button("X");
    if(this->showList){
        ImGui::DragFloat("Energy",&this->energy,1.0f,0.0f,1000,"%.1f");
    }
    ImGui::PopID();
    return res;
}

char Material::matLabelBuffer[] = "Material 0x______";