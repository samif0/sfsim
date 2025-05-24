#pragma once

#include <typeindex>
#include <memory>

namespace SFSim {
namespace ECS {

class Entity;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;
    
    virtual std::type_index getType() const = 0;
    
    Entity* getEntity() const { return _entity; }
    
    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void update(float deltaTime) {}
    
private:
    friend class Entity;
    Entity* _entity = nullptr;
};

template<typename T>
class ComponentBase : public Component {
public:
    std::type_index getType() const override {
        return std::type_index(typeid(T));
    }
};

} // namespace ECS
} // namespace SFSim