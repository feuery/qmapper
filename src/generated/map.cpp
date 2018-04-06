#include <map.h>
#include <mapContainer.h>
#include <json.hpp>
#include <QDebug>



#include <layerContainer.h>
////// generated at 2018-03-27T17:18:45.906Z


void Map::setName(std::string value) { 
Name_field = value;
for(auto fn: event_map["Name"]) { 
  fn.second(this->getId());
}
}
                                                        std::string Map::getName() const {
return Name_field;
}
void Map::setLayers(std::vector<Layer*>* value) { 
Layers_field = value;
for(auto fn: event_map["Layers"]) { 
  fn.second(this->getId());
}
}
                                                        std::vector<Layer*>* Map::getLayers() const {
return Layers_field;
}
Propertierbase* Map::copy() {
  Map* t = new Mapcontainer;
t->setId(getId());
t->setName(getName());
t->setLayers(getLayers()); 
return t;
}Map::Map() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("name")));
event_map["name"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("layers")));
event_map["layers"] = std::unordered_map<int, FUN>();
}Map* toMap(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Map")) {
  return static_cast<Map*>(b);
}
else {
printf("\"toMap called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Map::toJSON() const
{
nlohmann::json j;
auto G__16 = getId();
 j["Id"] = G__16;

auto G__17 = getName();
 j["Name"] = G__17;

auto G__18 = getLayers();
if(G__18)  j["Layers"] = *G__18;

;
return j.dump();
}
void Map::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setName(j["Name"]);
 getLayers()->clear();
for(auto it0 = j["Layers"].begin(); it0 != j["Layers"].end(); it0++) {

Layer *o = new Layercontainer;
std::string tmp = it0->dump();
const char *c_tmp = tmp.c_str();
o->fromJSON(c_tmp);getLayers()->push_back(o);
}
}

using nlohmann::json;

    void to_json(json& j, const Map& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Map& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Map* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Map*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Map>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<Map*>>* v) {
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

    void from_json(const json& j, Map* c) {
      std::string d = j.dump();
      // qDebug() << "JSON: " << d.c_str();
      try {
	c->fromJSON(d.c_str());
      }
      catch(...) {
	qDebug() << "Edellämainitun lataaminen falskaa";
	throw "";
      }
		    // get<std::string>().c_str());
}

void to_json(json& j, std::map<std::string, Map*>* m) {
  for(auto b = m->begin(); b != m->end(); m++) {
    json j2;
    to_json(j2, b->second);
    
    j[b->first] = j2;
  }
}
void from_json(const json& j, std::map<std::string, Map*>* m)
{
  for(auto b = j.begin(); b != j.end(); b++) {
    json j2 = b.value();
    Map *r = new Mapcontainer;
    from_json(j2, r);
    (*m)[b.key()] = r;
  }
}
