#include "ecs/entity.hpp"

namespace SFSim {
namespace ECS {

Entity::Entity(EntityID id) 
    : _id(id)
    , _active(true)
    , _name("Entity_" + std::to_string(id))
{
}

Entity::~Entity() {
    removeAllComponents();
}

void Entity::removeAllComponents() {
    for (auto& pair : _components) {
        pair.second->onDetach();
        pair.second->_entity = nullptr;
    }
    _components.clear();
}

std::vector<Component*> Entity::getAllComponents() {
    std::vector<Component*> components;
    components.reserve(_components.size());
    
    for (auto& pair : _components) {
        components.push_back(pair.second.get());
    }
    
    return components;
}

std::vector<const Component*> Entity::getAllComponents() const {
    std::vector<const Component*> components;
    components.reserve(_components.size());
    
    for (const auto& pair : _components) {
        components.push_back(pair.second.get());
    }
    
    return components;
}

void Entity::update(float deltaTime) {
    if (!_active) return;
    
    for (auto& pair : _components) {
        pair.second->update(deltaTime);
    }
}

} // namespace ECS
} // namespace SFSim