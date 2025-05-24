#pragma once

#include "component.hpp"
#include "geometry/geometry.hpp"
#include <memory>

namespace SFSim {
namespace ECS {

class RenderComponent : public ComponentBase<RenderComponent> {
public:
    RenderComponent();
    RenderComponent(std::unique_ptr<Geometry> geometry);
    
    ComponentType getComponentType() const override { return ComponentType::Render; }
    
    void setGeometry(std::unique_ptr<Geometry> geometry);
    Geometry* getGeometry() const { return _geometry.get(); }
    
    void setVisible(bool visible) { _visible = visible; }
    bool isVisible() const { return _visible; }
    
    void setColor(const sf::Color& color);
    
private:
    std::unique_ptr<Geometry> _geometry;
    bool _visible;
};

} // namespace ECS
} // namespace SFSim