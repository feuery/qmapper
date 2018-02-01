#ifndef Tilee
#define Tilee

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;

#include<boost/flyweight.hpp>
using namespace boost::flyweights;

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
int Rotation_field = 0;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, int value) {
if(propertyname == std::string("x") ) { X_field = value; return; }
if(propertyname == std::string("y") ) { Y_field = value; return; }
if(propertyname == std::string("rotation") ) { Rotation_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("tileset") ) { Tileset_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
virtual int get(flyweight<std::string> propertyname, bool *success, int type_helper) const {
if(propertyname == std::string("x")) {
  *success = true;
  return X_field;
}
if(propertyname == std::string("y")) {
  *success = true;
  return Y_field;
}
if(propertyname == std::string("rotation")) {
  *success = true;
  return Rotation_field;
} *success = false; int invalid_data; return invalid_data;
}
virtual std::string get(flyweight<std::string> propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("tileset")) {
  *success = true;
  return Tileset_field;
} *success = false; std::string invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Tile();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Tile"); }
virtual int property_count() { return 5; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) const {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("x")) return flyweight<std::string>("int");
if(propertyname == std::string("y")) return flyweight<std::string>("int");
if(propertyname == std::string("rotation")) return flyweight<std::string>("int");
if(propertyname == std::string("tileset")) return flyweight<std::string>("std::string");return flyweight<std::string>("");
}

};

Tile* toTile(Propertierbase *b);

    void to_json(json& j, const Tile& c);
    void from_json(const json& j, Tile& c);
    void to_json(json& j, const Tile* c);
    void from_json(const json& j, Tile* c);
    void to_json(json& j, const std::vector<Tile*>* v);
    void to_json(json& j, const std::vector<std::vector<Tile*>>* v);
    void to_json(json& j, const std::vector<std::vector<Tile>>* v);
#endif