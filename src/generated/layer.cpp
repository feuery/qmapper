#include <layer.h>
#include <json.hpp>
////// generated at 2018-02-01T09:49:48.762Z


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
Layer::Layer() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
r.push_back(flyweight<std::string>(std::string("opacity")));
r.push_back(flyweight<std::string>(std::string("visible")));
}Layer* toLayer(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Layer")) {
  return static_cast<Layer*>(b);
}
else {
printf("\"toLayer called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}

std::string Layer::toJSON() const
{
nlohmann::json j {
{"Id", getId().get()},
{"Name", getName()},
{"Opacity", getOpacity()}
};
return j.dump();
}
void Layer::fromJSON(const char* json)
{

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

    void from_json(const json& j, Layer* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
