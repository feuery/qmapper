#include <root.h>
#include <json.hpp>
////// generated at 2018-02-01T09:49:48.769Z


root::root() {
r.push_back(flyweight<std::string>(std::string("Id")));
}root* toRoot(Propertierbase *b)
 {
if(b->type_identifier() == std::string("root")) {
  return static_cast<root*>(b);
}
else {
printf("\"toRoot called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}

std::string root::toJSON() const
{
nlohmann::json j {
{"Id", getId().get()}
};
return j.dump();
}
void root::fromJSON(const char* json)
{

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

    void from_json(const json& j, root* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
