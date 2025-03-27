#pragma once

#include "imgui.h"

#include "material.hpp"
#include "machine.hpp"
#include "recipe.hpp"
#include "process.hpp"
#include "system.hpp"

#include <vector>
#include <cstddef>


enum class MENU : uint_fast8_t{
    MATERIAL,
    MACHINE,
    RECIPE,
    PROCESS,
    SYSTEM
};
struct State{
    struct deserializer{
        int index;
        std::byte *data;
        std::size_t length;
        template <class T>
        inline T get(){
            T res = *(T*)(this->peek());
            this->seek(sizeof(T));
            return res;
        }
        template <class T>
        inline deserializer &operator>>(T &dst){
            dst = *(T*)(this->peek());
            this->seek(sizeof(T));
            return *this;
        }
        inline std::byte *peek(){
            return this->data + this->index;
        }
        inline void seek(std::size_t count){
            this->index += count;
        }
    };
    std::vector<Material> materials;
    std::vector<Machine> machines;
    std::size_t serialize(std::byte **res);
    void deserialize(deserializer d);
private:
    std::vector<std::byte> buffer;
    template <class T>
    void appendBuffer(T obj);
};

void app();

void materialsMenu(State &state);
void machinesMenu(State &state);

void calculationMenu();