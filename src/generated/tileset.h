#ifndef Tilesete
#define Tilesete

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLFunctions>
#include<string>
#include<QString>
#include<script.h>
#include<renderer.h>
#include<tile.h>
#include<cstring>
class Script;
class Tileset: public Propertierbase {
 public: virtual void setName(std::string val);
virtual std::string getName();
std::string Name_field = "";
public: virtual void setVertexshader(Script* val);
virtual Script* getVertexshader();
Script* Vertexshader_field = nullptr;
public: virtual void setFragmentshader(Script* val);
virtual Script* getFragmentshader();
Script* Fragmentshader_field = nullptr;
public: virtual void setTiles(std::vector<std::vector<Tile*>>* val);
virtual std::vector<std::vector<Tile*>>* getTiles();
std::vector<std::vector<Tile*>>* Tiles_field = nullptr;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, Script* value) {
if(propertyname == std::string("vertexShader") ) { Vertexshader_field = value; return; }
if(propertyname == std::string("fragmentShader") ) { Fragmentshader_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::vector<std::vector<Tile*>>* value) {
if(propertyname == std::string("tiles") ) { Tiles_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
virtual std::string get(flyweight<std::string> propertyname, bool *success, std::string type_helper) {
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual Script* get(flyweight<std::string> propertyname, bool *success, Script* type_helper) {
if(propertyname == std::string("vertexShader")) {
  *success = true;
  return Vertexshader_field;
}
if(propertyname == std::string("fragmentShader")) {
  *success = true;
  return Fragmentshader_field;
} *success = false; Script* invalid_data; return invalid_data;
}
virtual std::vector<std::vector<Tile*>>* get(flyweight<std::string> propertyname, bool *success, std::vector<std::vector<Tile*>>* type_helper) {
if(propertyname == std::string("tiles")) {
  *success = true;
  return Tiles_field;
} *success = false; std::vector<std::vector<Tile*>>* invalid_data; return invalid_data;
}
public: Tileset();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Tileset"); }
virtual int property_count() { return 5; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("name")) return flyweight<std::string>("std::string");
if(propertyname == std::string("vertexShader")) return flyweight<std::string>("Script");
if(propertyname == std::string("fragmentShader")) return flyweight<std::string>("Script");
if(propertyname == std::string("tiles")) return flyweight<std::string>("std::vector<std::vector<Tile>>");return flyweight<std::string>("");
}

};

Tileset* toTileset(Propertierbase *b);
#endif