#include "renderer/material.hpp"
#include <cmath>
#include <algorithm>
#include <cstdint>

namespace SFSim {

Material::Material(MaterialType type)
    : _type(type)
    , _name("Default Material")
    , _diffuseColor(sf::Color::White)
    , _emissiveColor(sf::Color::Black)
    , _opacity(1.0f)
    , _wireframe(false)
    , _doubleSided(false)
{
}

sf::Color Material::calculateColor(const Vector3f& position, const Vector3f& normal, const Vector3f& lightDir) const {
    float NdotL = std::max(0.0f, normal.dot(lightDir));
    
    std::uint8_t r = static_cast<std::uint8_t>(_diffuseColor.r * NdotL + _emissiveColor.r);
    std::uint8_t g = static_cast<std::uint8_t>(_diffuseColor.g * NdotL + _emissiveColor.g);
    std::uint8_t b = static_cast<std::uint8_t>(_diffuseColor.b * NdotL + _emissiveColor.b);
    std::uint8_t a = static_cast<std::uint8_t>(_diffuseColor.a * _opacity);
    
    return sf::Color(r, g, b, a);
}

PhongMaterial::PhongMaterial()
    : Material(MaterialType::Phong)
    , _specularColor(sf::Color::White)
    , _ambientColor(sf::Color(32, 32, 32))
    , _shininess(32.0f)
{
}

sf::Color PhongMaterial::calculateColor(const Vector3f& position, const Vector3f& normal, const Vector3f& lightDir) const {
    Vector3f viewDir = Vector3f(0, 0, 1);
    Vector3f reflectDir = normal * (2.0f * normal.dot(lightDir)) - lightDir;
    
    float NdotL = std::max(0.0f, normal.dot(lightDir));
    float RdotV = std::max(0.0f, reflectDir.dot(viewDir));
    float specular = std::pow(RdotV, _shininess);
    
    float ambientStrength = 0.1f;
    float diffuseStrength = NdotL;
    float specularStrength = specular;
    
    std::uint8_t r = static_cast<std::uint8_t>(std::min(255.0f,
        _ambientColor.r * ambientStrength +
        _diffuseColor.r * diffuseStrength +
        _specularColor.r * specularStrength +
        _emissiveColor.r));
    
    std::uint8_t g = static_cast<std::uint8_t>(std::min(255.0f,
        _ambientColor.g * ambientStrength +
        _diffuseColor.g * diffuseStrength +
        _specularColor.g * specularStrength +
        _emissiveColor.g));
    
    std::uint8_t b = static_cast<std::uint8_t>(std::min(255.0f,
        _ambientColor.b * ambientStrength +
        _diffuseColor.b * diffuseStrength +
        _specularColor.b * specularStrength +
        _emissiveColor.b));
    
    std::uint8_t a = static_cast<std::uint8_t>(_diffuseColor.a * _opacity);
    
    return sf::Color(r, g, b, a);
}

PBRMaterial::PBRMaterial()
    : Material(MaterialType::PBR)
    , _metallic(0.0f)
    , _roughness(0.5f)
    , _specularF0(0.04f, 0.04f, 0.04f)
{
}

sf::Color PBRMaterial::calculateColor(const Vector3f& position, const Vector3f& normal, const Vector3f& lightDir) const {
    Vector3f viewDir = Vector3f(0, 0, 1);
    Vector3f halfVector = (lightDir + viewDir).normalized();
    
    float NdotL = std::max(0.0f, normal.dot(lightDir));
    float NdotV = std::max(0.0f, normal.dot(viewDir));
    float NdotH = std::max(0.0f, normal.dot(halfVector));
    float VdotH = std::max(0.0f, viewDir.dot(halfVector));
    
    Vector3f albedo = Vector3f(_diffuseColor.r / 255.0f, _diffuseColor.g / 255.0f, _diffuseColor.b / 255.0f);
    Vector3f F0 = Vector3f::lerp(_specularF0, albedo, _metallic);
    
    Vector3f F = fresnelSchlick(VdotH, F0);
    float D = distributionGGX(normal, halfVector, _roughness);
    float G = geometrySmith(normal, viewDir, lightDir, _roughness);
    
    Vector3f numerator = F * D * G;
    float denominator = 4.0f * NdotV * NdotL + 0.001f;
    Vector3f specular = numerator / denominator;
    
    Vector3f kS = F;
    Vector3f kD = Vector3f::one() - kS;
    kD = kD * (1.0f - _metallic);
    
    Vector3f diffuse = albedo / M_PI;
    Vector3f kD_diffuse(kD.x * diffuse.x, kD.y * diffuse.y, kD.z * diffuse.z);
    Vector3f color = (kD_diffuse + specular) * NdotL;
    
    std::uint8_t r = static_cast<std::uint8_t>(std::min(255.0f, color.x * 255.0f + _emissiveColor.r));
    std::uint8_t g = static_cast<std::uint8_t>(std::min(255.0f, color.y * 255.0f + _emissiveColor.g));
    std::uint8_t b = static_cast<std::uint8_t>(std::min(255.0f, color.z * 255.0f + _emissiveColor.b));
    std::uint8_t a = static_cast<std::uint8_t>(_diffuseColor.a * _opacity);
    
    return sf::Color(r, g, b, a);
}

Vector3f PBRMaterial::fresnelSchlick(float cosTheta, const Vector3f& F0) const {
    float oneMinusCos = 1.0f - cosTheta;
    float oneMinusCos5 = oneMinusCos * oneMinusCos * oneMinusCos * oneMinusCos * oneMinusCos;
    return F0 + (Vector3f::one() - F0) * oneMinusCos5;
}

float PBRMaterial::distributionGGX(const Vector3f& N, const Vector3f& H, float roughness) const {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = std::max(0.0f, N.dot(H));
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = M_PI * denom * denom;
    
    return num / denom;
}

float PBRMaterial::geometrySchlickGGX(float NdotV, float roughness) const {
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;
    
    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;
    
    return num / denom;
}

float PBRMaterial::geometrySmith(const Vector3f& N, const Vector3f& V, const Vector3f& L, float roughness) const {
    float NdotV = std::max(0.0f, N.dot(V));
    float NdotL = std::max(0.0f, N.dot(L));
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

} // namespace SFSim