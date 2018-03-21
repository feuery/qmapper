#ifndef Tilesete
#define Tilesete

#include<propertierbase.h>

#include<json.hpp>

using nlohmann::json;
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
virtual std::string getName() const;
std::string Name_field = "";
public: virtual void setVertexshader(Script* val);
virtual Script* getVertexshader() const;
Script* Vertexshader_field = nullptr;
public: virtual void setFragmentshader(Script* val);
virtual Script* getFragmentshader() const;
Script* Fragmentshader_field = nullptr;
public: virtual void setTiles(std::vector<std::vector<Tile*>>* val);
virtual std::vector<std::vector<Tile*>>* getTiles() const;
std::vector<std::vector<Tile*>>* Tiles_field = nullptr;virtual void set(std::string propertyname, std::string value) {
if(propertyname == std::string("Id") ) { setId(value); return; }
if(propertyname == std::string("name") ) { setName(value); return; } }
virtual void set(std::string propertyname, Script* value) {
if(propertyname == std::string("vertexShader") ) { setVertexshader(value); return; }
if(propertyname == std::string("fragmentShader") ) { setFragmentshader(value); return; } }
virtual void set(std::string propertyname, std::vector<std::vector<Tile*>>* value) {
if(propertyname == std::string("tiles") ) { setTiles(value); return; } }virtual std::string get(std::string propertyname, bool *success, std::string type_helper) const {
if(propertyname == std::string("Id")) {
  *success = true;
  return getId();
}
if(propertyname == std::string("name")) {
  *success = true;
  return getName();
} *success = false; std::string invalid_data; return invalid_data;
}
virtual Script* get(std::string propertyname, bool *success, Script* type_helper) const {
if(propertyname == std::string("vertexShader")) {
  *success = true;
  return getVertexshader();
}
if(propertyname == std::string("fragmentShader")) {
  *success = true;
  return getFragmentshader();
} *success = false; Script* invalid_data; return invalid_data;
}
virtual std::vector<std::vector<Tile*>>* get(std::string propertyname, bool *success, std::vector<std::vector<Tile*>>* type_helper) const {
if(propertyname == std::string("tiles")) {
  *success = true;
  return getTiles();
} *success = false; std::vector<std::vector<Tile*>>* invalid_data; return invalid_data;
}
public: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);
public: Tileset();

std::vector<std::string> r;
std::vector<std::string> names() { return r; }

virtual std::string type_identifier() { return std::string("Tileset"); }
virtual int property_count() { return 5; }
virtual std::string type_name(std::string propertyname) const {
if(propertyname == std::string("Id")) return std::string("std::string");
if(propertyname == std::string("name")) return std::string("std::string");
if(propertyname == std::string("vertexShader")) return std::string("Script");
if(propertyname == std::string("fragmentShader")) return std::string("Script");
if(propertyname == std::string("tiles")) return std::string("std::vector<std::vector<Tile>>");return std::string("");
}

};

Tileset* toTileset(Propertierbase *b);

    void to_json(json& j, const Tileset& c);
    void from_json(const json& j, Tileset& c);
    void to_json(json& j, const Tileset* c);
    void from_json(const json& j, Tileset* c);
    void to_json(json& j, const std::vector<Tileset*>* v);     void to_json(json& j, const std::vector<std::vector<Tileset*>>* v);
    void to_json(json& j, const std::vector<std::vector<Tileset>>* v);
void to_json(json& j, std::map<std::string, Tileset*>* m);
void from_json(const json& j, std::map<std::string, Tileset*>* m);

#else 
class Tileset;
#endif