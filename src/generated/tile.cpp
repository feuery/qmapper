#include <tile.h>
#include <json.hpp>
////// generated at 2018-03-11T13:11:30.833Z


void Tile::setTileset(std::string value) { 
Tileset_field = value;
for(auto fn: event_map["Tileset"]) { 
  fn.second(this);
}
}
                                                        std::string Tile::getTileset() const {
return Tileset_field;
}
void Tile::setX(int value) { 
X_field = value;
for(auto fn: event_map["X"]) { 
  fn.second(this);
}
}
                                                        int Tile::getX() const {
return X_field;
}
void Tile::setY(int value) { 
Y_field = value;
for(auto fn: event_map["Y"]) { 
  fn.second(this);
}
}
                                                        int Tile::getY() const {
return Y_field;
}
void Tile::setRotation(int value) { 
Rotation_field = value;
for(auto fn: event_map["Rotation"]) { 
  fn.second(this);
}
}
                                                        int Tile::getRotation() const {
return Rotation_field;
}
Tile::Tile() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("x")));
event_map["x"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("y")));
event_map["y"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("tileset")));
event_map["tileset"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("rotation")));
event_map["rotation"] = std::unordered_map<int, FUN>();
}Tile* toTile(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Tile")) {
  return static_cast<Tile*>(b);
}
else {
printf("\"toTile called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Tile::toJSON() const
{
nlohmann::json j {
{"Id", getId()},
{"X", getX()},
{"Y", getY()},
{"Tileset", getTileset()},
{"Rotation", getRotation()}
};
return j.dump();
}
void Tile::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setX(j["X"]);
setY(j["Y"]);
setTileset(j["Tileset"]);
setRotation(j["Rotation"]);
}

using nlohmann::json;

    void to_json(json& j, const Tile& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Tile& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Tile* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Tile*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Tile>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<Tile*>>* v) {
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

    void from_json(const json& j, Tile* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
