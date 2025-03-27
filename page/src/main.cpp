#include "main.hpp"

#include <emscripten.h>

#include <vector>
#include <string>

#include <iostream>
#include <cstddef>

State state;
MENU selectedMenu = MENU::MATERIAL;

EM_JS(void, JS_downloadFile, (std::byte *ptr, std::size_t length, const char *fileName), {
  downloadFile(new Uint8Array(Module.HEAPU8.buffer, ptr, length),UTF8ToString(fileName));
});
EM_JS(void, JS_uploadFile, (), {
  uploadFile((res)=>{
    try{
      let ptr = Module._malloc(res.length);
      Module.HEAPU8.set(res, ptr);
      Module._C_updateState(ptr,res.length);
      Module._free(ptr);
    }catch(e){
      console.error(e);
    }
  });
});

std::size_t State::serialize(std::byte **res){
  this->buffer.clear();

  this->appendBuffer(this->materials.size());
  this->appendBuffer(this->machines.size());

  for(Material m : this->materials){
    std::string temp{m.name};
    for(char c : temp){
      this->buffer.push_back((std::byte)c);
    }
    this->buffer.push_back((std::byte)'\0');
    this->appendBuffer(m.energy);
  }

  for(Machine m : this->machines){
    std::string temp{m.name};
    for(char c : temp){
      this->buffer.push_back((std::byte)c);
    }
    this->buffer.push_back((std::byte)'\0');
    this->appendBuffer(m.tiers.size());
    for(Machine::Tier t : m.tiers){
      this->appendBuffer(t);
    }
  }

  *res = this->buffer.data();
  return this->buffer.size();
}
template <class T>
void State::appendBuffer(const T obj){
  const std::byte *const o = (const std::byte *const)&obj;
  for(int i=0;i<sizeof(T);++i){
    this->buffer.push_back(o[i]);
  }
}

void State::deserialize(deserializer d){
  this->materials.clear();
  this->machines.clear();
  
  std::size_t matLength;// = d.get<std::size_t>();
  std::size_t macLength;// = d.get<std::size_t>();
  d >> matLength >> macLength;
  
  for(std::size_t i=0;i<matLength;++i){
    std::string str = std::string((char*)d.peek());
    d.seek(str.length()+1);
    std::cout << i << " : " << str << " : " << str.length() << std::endl;
    float energy;
    d >> energy;
    this->materials.push_back({i,"",energy});
    Material &m = *(this->materials.end()-1);
    memcpy(m.name,str.c_str(),str.size()+1);
  }
  for(std::size_t i=0;i<macLength;++i){
    std::string str = std::string((char*)d.peek());
    d.seek(str.length()+1);
    this->machines.push_back({i});
    Machine &m = *(this->machines.end()-1);
    memcpy(m.name,str.c_str(),str.size()+1);
    std::size_t tierCount;
    d >> tierCount;
    Machine::Tier tier;
    for(int i=0;i<tierCount;++i){
      d >> tier;
      m.tiers.push_back(tier);
    }
  }
}

EMSCRIPTEN_KEEPALIVE
extern "C" void C_updateState(std::byte *data, std::size_t length){
  state.deserialize({0,data,length});
}

void app(){
  ImGui::Begin("Elements Menu", nullptr, ImGuiWindowFlags_MenuBar);
  if(ImGui::BeginMenuBar()) {
    if(ImGui::MenuItem("Materials")){
      selectedMenu = MENU::MATERIAL;
    }
    if(ImGui::MenuItem("Machines")){
      selectedMenu = MENU::MACHINE;
    }
    if(ImGui::MenuItem("Recipes")){
      selectedMenu = MENU::RECIPE;
    }
    if(ImGui::MenuItem("Processes")){
      selectedMenu = MENU::PROCESS;
    }
    if(ImGui::MenuItem("System")){
      selectedMenu = MENU::SYSTEM;
    }
    if(ImGui::BeginMenu("File")){
      if(ImGui::MenuItem("Save")){
        std::byte *ptr;
        std::size_t len = state.serialize(&ptr);
        const char *fName = "test.txt";
        JS_downloadFile(ptr,len,fName);
      }
      if(ImGui::MenuItem("Load")){
        JS_uploadFile();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
  switch(selectedMenu){
    case MENU::MATERIAL:{
      materialsMenu(state);
    }break;
    case MENU::MACHINE:{
      machinesMenu(state);
    }break;
    default:{

    }
  }
  ImGui::End();
}