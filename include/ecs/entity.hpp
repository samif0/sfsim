#pragma once

#include "component.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

namespace SFSim {
namespace ECS {

using EntityID = std::size_t;

class Entity {
public:
    Entity(EntityID id);
    ~Entity();
    
    // Delete copy constructor and copy assignment operator
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    
    // Allow move constructor and move assignment operator
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    
    EntityID getId() const { return _id; }
    
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        
        std::type_index type = component->getType();
        
        if (_components.find(type) != _components.end()) {
            removeComponent<T>();
        }
        
        component->_entity = this;
        _components[type] = std::move(component);
        
        ptr->onAttach();
        return ptr;
    }
    
    template<typename T>
    T* getComponent() {
        std::type_index type = std::type_index(typeid(T));
        auto it = _components.find(type);
        if (it != _components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    
    template<typename T>
    const T* getComponent() const {
        std::type_index type = std::type_index(typeid(T));
        auto it = _components.find(type);
        if (it != _components.end()) {
            return static_cast<const T*>(it->second.get());
        }
        return nullptr;
    }
    
    template<typename T>
    bool hasComponent() const {
        std::type_index type = std::type_index(typeid(T));
        return _components.find(type) != _components.end();
    }
    
    template<typename T>
    void removeComponent() {
        std::type_index type = std::type_index(typeid(T));
        auto it = _components.find(type);
        if (it != _components.end()) {
            it->second->onDetach();
            it->second->_entity = nullptr;
            _components.erase(it);
        }
    }
    
    void removeAllComponents();
    
    std::vector<Component*> getAllComponents();
    std::vector<const Component*> getAllComponents() const;
    
    void update(float deltaTime);
    
    void setActive(bool active) { _active = active; }
    bool isActive() const { return _active; }
    
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    
    // Support for custom components using ComponentType enum
    void addCustomComponent(std::unique_ptr<Component> component) {
        component->_entity = this;
        _customComponents.push_back(std::move(component));
    }
    
    Component* getComponent(ComponentType type) {
        for (auto& comp : _customComponents) {
            if (comp->getComponentType() == type) {
                return comp.get();
            }
        }
        return nullptr;
    }
    
    const Component* getComponent(ComponentType type) const {
        for (const auto& comp : _customComponents) {
            if (comp->getComponentType() == type) {
                return comp.get();
            }
        }
        return nullptr;
    }
    
private:
    EntityID _id;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;
    std::vector<std::unique_ptr<Component>> _customComponents;
    bool _active;
    std::string _name;
};

} // namespace ECS
} // namespace SFSim