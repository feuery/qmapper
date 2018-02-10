#include <layer.h>
#include <layerContainer.h>
#include <json.hpp>
////// generated at 2018-02-04T16:23:50.974Z


void Layer::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Layer::getName() const {
return Name_field;
}
void Layer::setOpacity(unsigned char value) { 
Opacity_field = value;
}
                                                        unsigned char Layer::getOpacity() const {
return Opacity_field;
}
void Layer::setVisible(bool value) { 
Visible_field = value;
}
                                                        bool Layer::getVisible() const {
return Visible_field;
}
void Layer::setTiles(std::vector<std::vector<Tile>>* value) { 
Tiles_field = value;
}
                                                        std::vector<std::vector<Tile>>* Layer::getTiles() const {
return Tiles_field;
}
Layer::Layer() {
r.push_back(std::string(std::string("Id")));
r.push_back(std::string(std::string("name")));
r.push_back(std::string(std::string("opacity")));
r.push_back(std::string(std::string("visible")));
r.push_back(std::string(std::string("tiles")));
}Layer* toLayer(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Layer")) {
  return static_cast<Layer*>(b);
}
else {
printf("\"toLayer called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Layer::toJSON() const
{
nlohmann::json j {
{"Id", getId()},
{"Name", getName()},
{"Opacity", getOpacity()},
{"Tiles", getTiles()}
};
return j.dump();
}
void Layer::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setName(j["Name"]);
setOpacity(j["Opacity"]);
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

    void to_json(json& j, const Layer& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Layer& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Layer* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Layer*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Layer>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<Layer*>>* v) {
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

    void from_json(const json& j, Layer* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
