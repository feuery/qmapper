#include <tileset.h>
#include <json.hpp>
////// generated at 2018-03-10T15:42:38.951Z


void Tileset::setName(std::string value) { 
Name_field = value;
for(auto fn: event_map["Name"]) { 
  fn.second(this);
}
}
                                                        std::string Tileset::getName() const {
return Name_field;
}
void Tileset::setVertexshader(Script* value) { 
Vertexshader_field = value;
for(auto fn: event_map["Vertexshader"]) { 
  fn.second(this);
}
}
                                                        Script* Tileset::getVertexshader() const {
return Vertexshader_field;
}
void Tileset::setFragmentshader(Script* value) { 
Fragmentshader_field = value;
for(auto fn: event_map["Fragmentshader"]) { 
  fn.second(this);
}
}
                                                        Script* Tileset::getFragmentshader() const {
return Fragmentshader_field;
}
void Tileset::setTiles(std::vector<std::vector<Tile>>* value) { 
Tiles_field = value;
for(auto fn: event_map["Tiles"]) { 
  fn.second(this);
}
}
                                                        std::vector<std::vector<Tile>>* Tileset::getTiles() const {
return Tiles_field;
}
Tileset::Tileset() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("name")));
event_map["name"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("vertexShader")));
event_map["vertexShader"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("fragmentShader")));
event_map["fragmentShader"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("tiles")));
event_map["tiles"] = std::unordered_map<int, FUN>();
}Tileset* toTileset(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Tileset")) {
  return static_cast<Tileset*>(b);
}
else {
printf("\"toTileset called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Tileset::toJSON() const
{
nlohmann::json j {
{"Id", getId()},
{"Name", getName()},
{"Vertexshader", getVertexshader()},
{"Fragmentshader", getFragmentshader()},
{"Tiles", getTiles()}
};
return j.dump();
}
void Tileset::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setName(j["Name"]);
*getVertexshader() = j["Vertexshader"].get<Script>();
*getFragmentshader() = j["Fragmentshader"].get<Script>();
for(auto it0 = j["Tiles"].begin(); it0 != j["Tiles"].end(); it0++) {
std::vector<Tile> vec;
 
for(auto it1 = it0->begin(); it1 != it0->end(); it1++) {
Tile o;
std::string tmp = it1->dump();
const char *c_tmp = tmp.c_str();
o.fromJSON(c_tmp);                                                       vec.push_back(o);
}
                                                       getTiles()->push_back(vec);
}

}

using nlohmann::json;

    void to_json(json& j, const Tileset& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Tileset& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Tileset* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Tileset*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Tileset>>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           json j2;
           for(auto o = a->begin(); o != a->end(); o++) {
             j2.push_back(*o);
           }
           j.push_back(j2);
         }
    }
}

    void to_json(json& j, const std::vector<std::vector<Tileset*>>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           json j2;
           for(auto o = a->begin(); o != a->end(); o++) {
             j2.push_back(*o);
           }
           j.push_back(j2);
         }
    }
}

    void from_json(const json& j, Tileset* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
