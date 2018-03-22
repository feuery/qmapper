#include <tileset.h>
#include <json.hpp>






////// generated at 2018-03-22T17:33:33.064Z


void Tileset::setName(std::string value) { 
Name_field = value;
for(auto fn: event_map["Name"]) { 
  fn.second(this->getId());
}
}
                                                        std::string Tileset::getName() const {
return Name_field;
}
void Tileset::setVertexshader(Script* value) { 
Vertexshader_field = value;
for(auto fn: event_map["Vertexshader"]) { 
  fn.second(this->getId());
}
}
                                                        Script* Tileset::getVertexshader() const {
return Vertexshader_field;
}
void Tileset::setFragmentshader(Script* value) { 
Fragmentshader_field = value;
for(auto fn: event_map["Fragmentshader"]) { 
  fn.second(this->getId());
}
}
                                                        Script* Tileset::getFragmentshader() const {
return Fragmentshader_field;
}
void Tileset::setTiles(std::vector<std::vector<Tile*>>* value) { 
Tiles_field = value;
for(auto fn: event_map["Tiles"]) { 
  fn.second(this->getId());
}
}
                                                        std::vector<std::vector<Tile*>>* Tileset::getTiles() const {
return Tiles_field;
}
Propertierbase* Tileset::copy() {
  Tileset* t = new Tileset;
t->setId(getId());
t->setName(getName());
t->setVertexshader(getVertexshader());
t->setFragmentshader(getFragmentshader());
t->setTiles(getTiles()); 
return t;
}Tileset::Tileset() {
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
nlohmann::json j;
auto G__7010 = getId();
 j["Id"] = G__7010;

auto G__7011 = getName();
 j["Name"] = G__7011;

auto G__7012 = getVertexshader();
if(G__7012)  j["Vertexshader"] = *G__7012;

auto G__7013 = getFragmentshader();
if(G__7013)  j["Fragmentshader"] = *G__7013;

auto G__7014 = getTiles();
if(G__7014)  j["Tiles"] = *G__7014;

;
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
std::vector<Tile*> vec;
 
for(auto it1 = it0->begin(); it1 != it0->end(); it1++) {
Tile *o = new Tile;
std::string tmp = it1->dump();
const char *c_tmp = tmp.c_str();
o->fromJSON(c_tmp);                                                       vec.push_back(o);
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

void to_json(json& j, std::map<std::string, Tileset*>* m) {
  for(auto b = m->begin(); b != m->end(); m++) {
    json j2;
    to_json(j2, b->second);
    
    j[b->first] = j2;
  }
}
void from_json(const json& j, std::map<std::string, Tileset*>* m)
{
  for(auto b = j.begin(); b != j.end(); b++) {
    json j2 = b.value();
    Tileset *r = new Tileset;
    from_json(j2, r);
    (*m)[b.key()] = r;
  }
}