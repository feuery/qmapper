#include <sprite.h>
#include <spriteContainer.h>
#include <json.hpp>
////// generated at 2018-02-11T13:26:52.090Z


void Sprite::setParentmapid(std::string value) { 
Parentmapid_field = value;
}
                                                        std::string Sprite::getParentmapid() const {
return Parentmapid_field;
}
void Sprite::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Sprite::getName() const {
return Name_field;
}
Sprite::Sprite() {
r.push_back(std::string(std::string("Id")));
r.push_back(std::string(std::string("parentMapId")));
r.push_back(std::string(std::string("name")));
}Sprite* toSprite(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Sprite")) {
  return static_cast<Sprite*>(b);
}
else {
printf("\"toSprite called with \"%s\"\n", b->type_identifier().c_str());
throw "";
}
}

std::string Sprite::toJSON() const
{
nlohmann::json j {
{"Id", getId()},
{"Parentmapid", getParentmapid()},
{"Name", getName()}
};
return j.dump();
}
void Sprite::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
setParentmapid(j["Parentmapid"]);
setName(j["Name"]);
}

using nlohmann::json;

    void to_json(json& j, const Sprite& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Sprite& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Sprite* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void to_json(json& j, const std::vector<Sprite*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<Sprite>>* v) {
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

    void to_json(json& j, const std::vector<std::vector<Sprite*>>* v) {
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

    void from_json(const json& j, Sprite* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
