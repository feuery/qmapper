#include <root.h>
#include <rootContainer.h>
#include <json.hpp>









////// generated at 2018-03-27T17:18:45.901Z


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
auto G__8 = getId();
 j["Id"] = G__8;

auto G__9 = getAnimatedsprites();
if(G__9)  j["Animatedsprites"] = *G__9;

auto G__10 = getLayers();
if(G__10)  j["Layers"] = *G__10;

auto G__11 = getMaps();
if(G__11)  j["Maps"] = *G__11;

auto G__12 = getScripts();
if(G__12)  j["Scripts"] = *G__12;

auto G__13 = getSprites();
if(G__13)  j["Sprites"] = *G__13;

auto G__14 = getTiles();
if(G__14)  j["Tiles"] = *G__14;

auto G__15 = getTilesets();
if(G__15)  j["Tilesets"] = *G__15;

;
return j.dump();
}
void root::fromJSON(const char* json_str)
{
  json j = json::parse(json_str);
  setId(j["Id"]);
  auto an = getAnimatedsprites();
  an->clear();
  if(an) 
    from_json(j["Animatedsprites"], an);
  auto l = getLayers();
  l->clear();
  if(l)
    from_json(j["Layers"], l);
  getMaps()->clear();
  from_json(j["Maps"], getMaps());
  try {
    std::map<std::string, Script*>* scripts = getScripts();
    scripts->clear();
    if(scripts)
      from_json(j["Scripts"], scripts);
  }
  catch(...) {
    puts("Skriptien lataus kusee");
    throw "";
  }
  auto spr =  getSprites();
  spr->clear();
  if(spr)
    from_json(j["Sprites"], spr);
  auto tils = getTiles();
  tils->clear();
  if(tils)
    from_json(j["Tiles"], tils);
  std::map<std::string, Tileset*>* tilsets =  getTilesets();
  tilsets->clear();
  std::string jtilesets = j["Tilesets"].dump();

  // TÄÄ FEILAAokei 
  if(tilsets)
    from_json(j["Tilesets"],tilsets);
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
