#ifndef OBJ_HPP
#define OBJ_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>

class obj {
private:
    unsigned long id;
    sf::VertexArray shp;
    
public:
    obj(unsigned long _id):id(_id){};
    obj(unsigned long _id, sf::VertexArray _shp): id(_id), shp(_shp) {};
    ~obj(){};

    unsigned long g_id(){ return this->id; };
    sf::VertexArray g_shp(){ return this->shp; };
};

struct sim_objects {
    std::unordered_map<unsigned long, obj> objects;
};

#endif