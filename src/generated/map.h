#ifndef Mape
#define Mape

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<root.h>
#include<layer.h>
#include<vector>
#include<string>
#include<resize-enums.h>
#include<animatedSprite.h>
#include<cstring>
class root;
class Layer;
class Texture;
class Map: public Propertierbase {
 public: virtual void setName(std::string val);
virtual std::string getName() const;
std::string Name_field = "Map 1";
public: virtual void setLayers(std::vector<Layer*>* val);
virtual std::vector<Layer*>* getLayers() const;
std::vector<Layer*>* Layers_field = nullptr;
public: virtual either<animatedsprite*, Sprite*> findNearest (int x, int y) = 0;
public: virtual void parent (root* p) = 0;
public: virtual int width () = 0;
public: virtual int height () = 0;
public: virtual void resize (int w, int h, verticalAnchor vAnchor, horizontalAnchor hAnchor) = 0;
public: virtual root* parent () = 0;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; }
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(std::string propertyname, std::vector<Layer*>* value) {
if(propertyname == std::string("layers") ) { Layers_field = value; return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
}
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual std::vector<Layer*>* get(std::string propertyname, bool *success, std::vector<Layer*>* type_helper) const {
if(propertyname == std::string("layers")) {
  *success = true;
  return Layers_field;
} *success = false; std::vector<Layer*>* invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Map();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("Map"); }
virtual int property_count() { return 3; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("name")) return std::string("std::string");
if(propertyname == std::string("layers")) return std::string("std::vector<Layer>");return std::string("");
}

};

Map* toMap(Propertierbase *b);

    void to_json(json& j, const Map& c);
    void from_json(const json& j, Map& c);
    void to_json(json& j, const Map* c);
    void from_json(const json& j, Map* c);
    void to_json(json& j, const std::vector<Map*>* v);     void to_json(json& j, const std::vector<std::vector<Map*>>* v);
    void to_json(json& j, const std::vector<std::vector<Map>>* v);
#endif