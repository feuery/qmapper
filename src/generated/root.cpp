#include <QDebug>
#include <root.h>
#include <rootContainer.h>
#include <json.hpp>
#include <tileset.h>
#include <map.h>
////// generated at 2018-03-13T16:43:45.008Z


void root::setRegistry(std::map<std::string, std::map<std::string, Propertierbase*>>* value) { 
Registry_field = value;
for(auto fn: event_map["Registry"]) { 
  fn.second(this->getId());
}
}
                                                        std::map<std::string, std::map<std::string, Propertierbase*>>* root::getRegistry() const {
return Registry_field;
}
root::root() {
r.push_back(std::string(std::string("Id")));
event_map["Id"] = std::unordered_map<int, FUN>();
r.push_back(std::string(std::string("registry")));
event_map["registry"] = std::unordered_map<int, FUN>();
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
nlohmann::json j {
{"Id", getId()},
{"Registry", getRegistry()}
};
return j.dump();
}
void root::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
setId(j["Id"]);
 puts("TODO IMPLEMENT");
 throw "";
// *getRegistry() = j["Registry"].get<std::map<std::string, std::map<std::string, Propertierbase*>>>();
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

void to_json(json& j, Propertierbase*& b)
{
  std::string t = b->type_identifier();
  if(t == "root") {
    to_json(j, static_cast<root*>(b));

  }
  else if (t == "Map") {
    to_json(j, static_cast<Map*>(b));
  }
  else if(t == "Script") {
    to_json(j, static_cast<Script*>(b));
  }
  else if(t == "Tileset") {
    to_json(j, static_cast<Tileset*>(b));
  }
  else if (t == "Sprite") {
    to_json(j, static_cast<Sprite*>(b));
  }
  else {
    qDebug() << "Didn't recogniza type " << t.c_str() << " at " << __FILE__ << ":" << __LINE__;
  }
}

void to_json(json& j, const std::map<std::string, std::map<std::string, Propertierbase*> >* m)
{
  for(auto a = m->begin(); a != m->end(); a++) {
    std::string k1 = a->first;
    json j2;
    for(auto a2: a->second) {
      std::string k2 = a2.first;
      j2[k2] = a2.second;
    }
    j[k1] = j2;
  }
}

void from_json(const json& j, std::map<std::string, std::map<std::string, Propertierbase*> >* m) {
  puts("Don't call me yet, TODO IMPLEMENT");
  throw "";
}
