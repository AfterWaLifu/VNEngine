#pragma once

#include <VNEngine/LuaReaders/LuaReader.hpp>

#include <string>

namespace VNEngine {

class InterfaceCreator : public LuaReader {

public:
    InterfaceCreator();
    ~InterfaceCreator();

    void Draw(const std::string& what);
};
} // namespace VNEngine