#ifndef Tilee
#define Tilee

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;

#include<cstring>
;
class Tile: public Propertierbase {
 public: virtual void setX(int val);
virtual int getX() const;
int X_field = 0;
public: virtual void setY(int val);
virtual int getY() const;
int Y_field = 0;
public: virtual void setTileset(std::string val);
virtual std::string getTileset() const;
std::string Tileset_field;
public: virtual void setRotation(int val);
virtual int getRotation() const;
int Rotation_field = 0;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { setId(value); return; }
if(propertyname == std::string("tileset") ) { setTileset(value); return; } }
virtual void set(std::string propertyname, int value) {
if(propertyname == std::string("x") ) { setX(value); return; }
if(propertyname == std::string("y") ) { setY(value); return; }
if(propertyname == std::string("rotation") ) { setRotation(value); return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return getId();
}
if(propertyname == std::string("tileset")) {
  *success = true;
  return getTileset();
} *success = false; std::string invalid_data; return invalid_data;
}
virtual int get(std::string propertyname, bool *success, int type_helper) const {
if(propertyname == std::string("x")) {
  *success = true;
  return getX();
}
if(propertyname == std::string("y")) {
  *success = true;
  return getY();
}
if(propertyname == std::string("rotation")) {
  *success = true;
  return getRotation();
} *success = false; int invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Tile();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("Tile"); }
virtual int property_count() { return 5; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("tileset")) return std::string("std::string");
if(propertyname == std::string("x")) return std::string("int");
if(propertyname == std::string("y")) return std::string("int");
if(propertyname == std::string("rotation")) return std::string("int");return std::string("");
}

};

Tile* toTile(Propertierbase *b);

    void to_json(json& j, const Tile& c);
    void from_json(const json& j, Tile& c);
    void to_json(json& j, const Tile* c);
    void from_json(const json& j, Tile* c);
    void to_json(json& j, const std::vector<Tile*>* v);     void to_json(json& j, const std::vector<std::vector<Tile*>>* v);
    void to_json(json& j, const std::vector<std::vector<Tile>>* v);
#endif