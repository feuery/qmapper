#include <root.h>
#include <rootContainer.h>
#include <json.hpp>









////// generated at 2018-03-21T17:37:13.708Z


void root::setAnimatedsprites(std::map<std::string, animatedsprite*>* value) { 
Animatedsprites_field = value;
for(auto fn: event_map["Animatedsprites"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, animatedsprite*>* root::getAnimatedsprites() const {
return Animatedsprites_field;
}
void root::setLayers(std::map<std::string, Layer*>* value) { 
Layers_field = value;
for(auto fn: event_map["Layers"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, Layer*>* root::getLayers() const {
return Layers_field;
}
void root::setMaps(std::map<std::string, Map*>* value) { 
Maps_field = value;
for(auto fn: event_map["Maps"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, Map*>* root::getMaps() const {
return Maps_field;
}
void root::setScripts(std::map<std::string, Script*>* value) { 
Scripts_field = value;
for(auto fn: event_map["Scripts"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, Script*>* root::getScripts() const {
return Scripts_field;
}
void root::setSprites(std::map<std::string, Sprite*>* value) { 
Sprites_field = value;
for(auto fn: event_map["Sprites"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, Sprite*>* root::getSprites() const {
return Sprites_field;
}
void root::setTiles(std::map<std::string, Tile*>* value) { 
Tiles_field = value;
for(auto fn: event_map["Tiles"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, Tile*>* root::getTiles() const {
return Tiles_field;
}
void root::setTilesets(std::map<std::string, Tileset*>* value) { 
Tilesets_field = value;
for(auto fn: event_map["Tilesets"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, Tileset*>* root::getTilesets() const {
return Tilesets_field;
}
root::root() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("animatedSprites")));
event_map["animatedSprites"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("layers")));
event_map["layers"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("maps")));
event_map["maps"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("scripts")));
event_map["scripts"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("sprites")));
event_map["sprites"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("tiles")));
event_map["tiles"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("tilesets")));
event_map["tilesets"] = std::unordered_map<int, FUN>();
}root* toRoot(Propertierbase *b)
 {
if(b->type_identifier() == std::string("root")) {
  return static_cast<root*>(b);
}
else {
printf("\"toRoot called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string root::toJSON() const
{
nlohmann::json j;
auto G__5385 = getId();
 j["Id"] = G__5385;

auto G__5386 = getAnimatedsprites();
if(G__5386)  j["Animatedsprites"] = *G__5386;

auto G__5387 = getLayers();
if(G__5387)  j["Layers"] = *G__5387;

auto G__5388 = getMaps();
if(G__5388)  j["Maps"] = *G__5388;

auto G__5389 = getScripts();
if(G__5389)  j["Scripts"] = *G__5389;

auto G__5390 = getSprites();
if(G__5390)  j["Sprites"] = *G__5390;

auto G__5391 = getTiles();
if(G__5391)  j["Tiles"] = *G__5391;

auto G__5392 = getTilesets();
if(G__5392)  j["Tilesets"] = *G__5392;

;
return j.dump();
}
void root::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
from_json(j["Animatedsprites"], getAnimatedsprites());
from_json(j["Layers"], getLayers());
from_json(j["Maps"], getMaps());
from_json(j["Scripts"], getScripts());
from_json(j["Sprites"], getSprites());
from_json(j["Tiles"], getTiles());
from_json(j["Tilesets"], getTilesets());
}

using nlohmann::json;

    void to_json(json& j, const root& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, root& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const root* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<root*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<root>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<root*>>* v) {
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

    void from_json(const json& j, root* c) {
        c->fromJSON(j.get<std::string>().c_str());
}

void to_json(json& j, std::map<std::string, root*>* m) {
  for(auto b = m->begin(); b != m->end(); m++) {
    json j2;
    to_json(j2, b->second);
    
    j[b->first] = j2;
  }
}
void from_json(const json& j, std::map<std::string, root*>* m)
{
  for(auto b = j.begin(); b != j.end(); b++) {
    json j2 = b.value();
    root *r = new Rootcontainer;
    from_json(j2, r);
    (*m)[b.key()] = r;
  }
}