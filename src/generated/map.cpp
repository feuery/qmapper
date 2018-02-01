#include <map.h>
#include <json.hpp>
////// generated at 2018-02-01T11:28:50.279Z


void Map::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Map::getName() const {
return Name_field;
}
void Map::setText(Texture* value) { 
Text_field = value;
}
                                                        Texture* Map::getText() const {
return Text_field;
}
void Map::setLayers(std::vector<Layer*>* value) { 
Layers_field = value;
}
                                                        std::vector<Layer*>* Map::getLayers() const {
return Layers_field;
}
Map::Map() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
r.push_back(flyweight<std::string>(std::string("text")));
r.push_back(flyweight<std::string>(std::string("layers")));
}Map* toMap(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Map")) {
  return static_cast<Map*>(b);
}
else {
printf("\"toMap called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}

std::string Map::toJSON() const
{
nlohmann::json j {
{"Id", getId().get()},
{"Name", getName()},
{"Text", getText()},
{"Layers", getLayers()}
};
return j.dump();
}
void Map::fromJSON(const char* json)
{

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
        c->fromJSON(j.get<std::string>().c_str());
}
