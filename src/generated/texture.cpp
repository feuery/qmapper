#include <texture.h>
#include <json.hpp>
////// generated at 2018-02-01T09:49:48.745Z


void Texture::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Texture::getName() const {
return Name_field;
}
Texture::Texture() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
}Texture* toTexture(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Texture")) {
  return static_cast<Texture*>(b);
}
else {
printf("\"toTexture called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}

std::string Texture::toJSON() const
{
nlohmann::json j {
{"Id", getId().get()},
{"Name", getName()}
};
return j.dump();
}
void Texture::fromJSON(const char* json)
{

}

using nlohmann::json;

    void to_json(json& j, const Texture& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Texture& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Texture* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void from_json(const json& j, Texture* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
