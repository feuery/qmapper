#ifndef Mape
#define Mape

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<root.h>
#include<layer.h>
#include<vector>
#include<string>
#include<texture.h>
#include<resize-enums.h>
#include<cstring>
class root;
class Layer;
class Texture;
class Map: public Propertierbase {
 public: virtual void setName(std::string val);
virtual std::string getName() const;
std::string Name_field = "Map 1";
public: virtual void setText(Texture* val);
virtual Texture* getText() const;
Texture* Text_field = nullptr;
public: virtual void setLayers(std::vector<Layer*>* val);
virtual std::vector<Layer*>* getLayers() const;
std::vector<Layer*>* Layers_field = nullptr;
public: virtual void parent (root* p) = 0;
public: virtual int width () = 0;
public: virtual int height () = 0;
public: virtual void resize (int w, int h, verticalAnchor vAnchor, horizontalAnchor hAnchor) = 0;
public: virtual root* parent () = 0;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, Texture* value) {
if(propertyname == std::string("text") ) { Text_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::vector<Layer*>* value) {
if(propertyname == std::string("layers") ) { Layers_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
virtual std::string get(flyweight<std::string> propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual Texture* get(flyweight<std::string> propertyname, bool *success, Texture* type_helper) const {
if(propertyname == std::string("text")) {
  *success = true;
  return Text_field;
} *success = false; Texture* invalid_data; return invalid_data;
}
virtual std::vector<Layer*>* get(flyweight<std::string> propertyname, bool *success, std::vector<Layer*>* type_helper) const {
if(propertyname == std::string("layers")) {
  *success = true;
  return Layers_field;
} *success = false; std::vector<Layer*>* invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Map();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Map"); }
virtual int property_count() { return 4; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) const {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("name")) return flyweight<std::string>("std::string");
if(propertyname == std::string("text")) return flyweight<std::string>("Texture");
if(propertyname == std::string("layers")) return flyweight<std::string>("std::vector<Layer>");return flyweight<std::string>("");
}

};

Map* toMap(Propertierbase *b);

    void to_json(json& j, const Map& c);
    void from_json(const json& j, Map& c);
    void to_json(json& j, const Map* c);
    void from_json(const json& j, Map* c);
    void to_json(json& j, const std::vector<Map*>* v);
    void to_json(json& j, const std::vector<std::vector<Map*>>* v);
    void to_json(json& j, const std::vector<std::vector<Map>>* v);
#endif