#include <root.h>
#include <rootContainer.h>
#include <json.hpp>









////// generated at 2018-04-08T18:07:57.158Z


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
Propertierbase* root::copy() {
  root* t = new Rootcontainer;
t->setId(getId());
t->setAnimatedsprites(getAnimatedsprites());
t->setLayers(getLayers());
t->setMaps(getMaps());
t->setScripts(getScripts());
t->setSprites(getSprites());
t->setTiles(getTiles());
t->setTilesets(getTilesets()); 
return t;
}root::root() {
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
auto G__61 = getId();
 j["Id"] = G__61;

auto G__62 = getAnimatedsprites();
if(G__62)  j["Animatedsprites"] = *G__62;

auto G__63 = getLayers();
if(G__63)  j["Layers"] = *G__63;

auto G__64 = getMaps();
if(G__64)  j["Maps"] = *G__64;

auto G__65 = getScripts();
if(G__65)  j["Scripts"] = *G__65;

auto G__66 = getSprites();
if(G__66)  j["Sprites"] = *G__66;

auto G__67 = getTiles();
if(G__67)  j["Tiles"] = *G__67;

auto G__68 = getTilesets();
if(G__68)  j["Tilesets"] = *G__68;

;
return j.dump();
}
void root::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"].get<std::string>());
auto G__69 = getAnimatedsprites();
 G__69->clear();
 if(G__69) from_json(j["Animatedsprites"], G__69);
auto G__70 = getLayers();
 G__70->clear();
 if(G__70) from_json(j["Layers"], G__70);
auto G__71 = getMaps();
 G__71->clear();
 if(G__71) from_json(j["Maps"], G__71);
auto G__72 = getScripts();
 G__72->clear();
 if(G__72) from_json(j["Scripts"], G__72);
auto G__73 = getSprites();
 G__73->clear();
 if(G__73) from_json(j["Sprites"], G__73);
auto G__74 = getTiles();
 G__74->clear();
 if(G__74) from_json(j["Tiles"], G__74);
auto G__75 = getTilesets();
 G__75->clear();
 if(G__75) from_json(j["Tilesets"], G__75);
}

using nlohmann::json;

    void to_json(json& j, const root& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, root& c) {
        c.fromJSON(j.dump().c_str());
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
        c->fromJSON(j.dump().c_str());
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
    root *r = new Rootcontainer;
    from_json(*b, r);
    (*m)[b.key()] = r;
  }
}