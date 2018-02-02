#ifndef Layere
#define Layere

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
#include<tile.h>
#include<string>
#include<vector>
#include<map.h>
#include<cstring>
class Map;
class Tile;
class Layer: public Propertierbase {
 public: virtual void setName(std::string val);
virtual std::string getName() const;
std::string Name_field = "";
public: virtual void setOpacity(unsigned char val);
virtual unsigned char getOpacity() const;
unsigned char Opacity_field = 255;
public: virtual void setVisible(bool val);
virtual bool getVisible() const;
bool Visible_field = true;
public: virtual void setTiles(std::vector<std::vector<Tile>>* val);
virtual std::vector<std::vector<Tile>>* getTiles() const;
std::vector<std::vector<Tile>>* Tiles_field = nullptr;
public: virtual void set_parent (Map* p) = 0;
public: virtual int getWidth () = 0;
public: virtual int getHeight () = 0;
public: virtual Map* parent () = 0;
protected: int w = 0;
protected: int h = 0;
protected: Map* p = nullptr;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; }
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(std::string propertyname, unsigned char value) {
if(propertyname == std::string("opacity") ) { Opacity_field = value; return; } }
virtual void set(std::string propertyname, bool value) {
if(propertyname == std::string("visible") ) { Visible_field = value; return; } }
virtual void set(std::string propertyname, std::vector<std::vector<Tile>>* value) {
if(propertyname == std::string("tiles") ) { Tiles_field = value; return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
}
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual unsigned char get(std::string propertyname, bool *success, unsigned char type_helper) const {
if(propertyname == std::string("opacity")) {
  *success = true;
  return Opacity_field;
} *success = false; unsigned char invalid_data; return invalid_data;
}
virtual bool get(std::string propertyname, bool *success, bool type_helper) const {
if(propertyname == std::string("visible")) {
  *success = true;
  return Visible_field;
} *success = false; bool invalid_data; return invalid_data;
}
virtual std::vector<std::vector<Tile>>* get(std::string propertyname, bool *success, std::vector<std::vector<Tile>>* type_helper) const {
if(propertyname == std::string("tiles")) {
  *success = true;
  return Tiles_field;
} *success = false; std::vector<std::vector<Tile>>* invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Layer();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("Layer"); }
virtual int property_count() { return 5; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("name")) return std::string("std::string");
if(propertyname == std::string("opacity")) return std::string("unsigned char");
if(propertyname == std::string("visible")) return std::string("bool");
if(propertyname == std::string("tiles")) return std::string("std::vector<std::vector<Tile>>");return std::string("");
}

};

Layer* toLayer(Propertierbase *b);

    void to_json(json& j, const Layer& c);
    void from_json(const json& j, Layer& c);
    void to_json(json& j, const Layer* c);
    void from_json(const json& j, Layer* c);
    void to_json(json& j, const std::vector<Layer*>* v);     void to_json(json& j, const std::vector<std::vector<Layer*>>* v);
    void to_json(json& j, const std::vector<std::vector<Layer>>* v);
#endif