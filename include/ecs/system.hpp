#pragma once

#include "entity.hpp"
#include <vector>
#include <memory>

namespace SFSim {
namespace ECS {

class System {
public:
    System() = default;
    virtual ~System() = default;
    
    virtual void update(float deltaTime, const std::vector<std::unique_ptr<Entity>>& entities) = 0;
    virtual void onEntityAdded(Entity* entity) {}
    virtual void onEntityRemoved(Entity* entity) {}
    
protected:
    template<typename... ComponentTypes>
    std::vector<Entity*> getEntitiesWith(const std::vector<std::unique_ptr<Entity>>& entities) {
        std::vector<Entity*> result;
        
        for (const auto& entity : entities) {
            if (entity->isActive() && hasAllComponents<ComponentTypes...>(entity.get())) {
                result.push_back(entity.get());
            }
        }
        
        return result;
    }
    
private:
    template<typename T>
    bool hasAllComponents(Entity* entity) {
        return entity->hasComponent<T>();
    }
    
    template<typename T, typename U, typename... Rest>
    bool hasAllComponents(Entity* entity) {
        return entity->hasComponent<T>() && hasAllComponents<U, Rest...>(entity);
    }
};

} // namespace ECS
} // namespace SFSim