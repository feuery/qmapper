#include <tileset.h>
#include <tilesetContainer.h>
#include <json.hpp>
#include <QDebug>





////// generated at 2018-03-27T17:18:45.920Z


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

 setTiles(new std::vector<std::vector<Tile*>>);
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
auto G__45 = getId();
 j["Id"] = G__45;

auto G__46 = getName();
 j["Name"] = G__46;

auto G__47 = getVertexshader();
if(G__47)  j["Vertexshader"] = *G__47;

auto G__48 = getFragmentshader();
if(G__48)  j["Fragmentshader"] = *G__48;

auto G__49 = getTiles();
if(G__49)  j["Tiles"] = *G__49;

;
return j.dump();
}
void Tileset::fromJSON(const char* json_str)
{
  json j = json::parse(json_str);
  std::string id = j["Id"].get<std::string>(); 
  printf("Tilesetin ID: %s\n", id.c_str());
  setId(id);
  setName(j["Name"].get<std::string>());

  // puts("Lol 0");
  // Script *scr = toScript(j["Vertexshader"].get<Script>().copy());
  //   puts("Lol 1");
  // setVertexshader(scr);
  //   puts("Lol 2");
  // scr = toScript(j["Fragmentshader"].get<Script>().copy());
  //   puts("Lol 3");
  // setFragmentshader(scr);
  //   puts("Lol 4");
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
        c.fromJSON(j.dump().c_str());
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
      std::string dump = j.dump();
      // try {
        c->fromJSON(dump.c_str());
      // }
      // catch(...) {
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
    Tileset *r = new tilesetContainer;
    from_json(*b, r);
    (*m)[b.key()] = r;
  }
}
