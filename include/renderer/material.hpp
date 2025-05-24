#pragma once

#include "math/vector.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace SFSim {

using namespace Math;

enum class MaterialType {
    Basic,
    Phong,
    PBR
};

class Material {
public:
    Material(MaterialType type = MaterialType::Basic);
    virtual ~Material() = default;
    
    MaterialType getType() const { return _type; }
    
    void setName(const std::string& name) { _name = name; }
    const std::string& getName() const { return _name; }
    
    void setDiffuseColor(const sf::Color& color) { _diffuseColor = color; }
    const sf::Color& getDiffuseColor() const { return _diffuseColor; }
    
    void setEmissiveColor(const sf::Color& color) { _emissiveColor = color; }
    const sf::Color& getEmissiveColor() const { return _emissiveColor; }
    
    void setOpacity(float opacity) { _opacity = std::max(0.0f, std::min(1.0f, opacity)); }
    float getOpacity() const { return _opacity; }
    
    void setWireframe(bool wireframe) { _wireframe = wireframe; }
    bool isWireframe() const { return _wireframe; }
    
    void setDoubleSided(bool doubleSided) { _doubleSided = doubleSided; }
    bool isDoubleSided() const { return _doubleSided; }
    
    virtual sf::Color calculateColor(const Vector3f& position, const Vector3f& normal, const Vector3f& lightDir) const;
    
protected:
    MaterialType _type;
    std::string _name;
    sf::Color _diffuseColor;
    sf::Color _emissiveColor;
    float _opacity;
    bool _wireframe;
    bool _doubleSided;
};

class PhongMaterial : public Material {
public:
    PhongMaterial();
    
    void setSpecularColor(const sf::Color& color) { _specularColor = color; }
    const sf::Color& getSpecularColor() const { return _specularColor; }
    
    void setShininess(float shininess) { _shininess = std::max(1.0f, shininess); }
    float getShininess() const { return _shininess; }
    
    void setAmbientColor(const sf::Color& color) { _ambientColor = color; }
    const sf::Color& getAmbientColor() const { return _ambientColor; }
    
    sf::Color calculateColor(const Vector3f& position, const Vector3f& normal, const Vector3f& lightDir) const override;
    
private:
    sf::Color _specularColor;
    sf::Color _ambientColor;
    float _shininess;
};

class PBRMaterial : public Material {
public:
    PBRMaterial();
    
    void setMetallic(float metallic) { _metallic = std::max(0.0f, std::min(1.0f, metallic)); }
    float getMetallic() const { return _metallic; }
    
    void setRoughness(float roughness) { _roughness = std::max(0.0f, std::min(1.0f, roughness)); }
    float getRoughness() const { return _roughness; }
    
    void setSpecularF0(const Vector3f& f0) { _specularF0 = f0; }
    const Vector3f& getSpecularF0() const { return _specularF0; }
    
    sf::Color calculateColor(const Vector3f& position, const Vector3f& normal, const Vector3f& lightDir) const override;
    
private:
    float _metallic;
    float _roughness;
    Vector3f _specularF0;
    
    Vector3f fresnelSchlick(float cosTheta, const Vector3f& F0) const;
    float distributionGGX(const Vector3f& N, const Vector3f& H, float roughness) const;
    float geometrySchlickGGX(float NdotV, float roughness) const;
    float geometrySmith(const Vector3f& N, const Vector3f& V, const Vector3f& L, float roughness) const;
};

} // namespace SFSim