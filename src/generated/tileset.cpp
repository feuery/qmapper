#include <tileset.h>
#include <json.hpp>
////// generated at 2018-02-01T09:49:48.753Z


void Tileset::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Tileset::getName() const {
return Name_field;
}
void Tileset::setVertexshader(Script* value) { 
Vertexshader_field = value;
}
                                                        Script* Tileset::getVertexshader() const {
return Vertexshader_field;
}
void Tileset::setFragmentshader(Script* value) { 
Fragmentshader_field = value;
}
                                                        Script* Tileset::getFragmentshader() const {
return Fragmentshader_field;
}
void Tileset::setTiles(std::vector<std::vector<Tile*>>* value) { 
Tiles_field = value;
}
                                                        std::vector<std::vector<Tile*>>* Tileset::getTiles() const {
return Tiles_field;
}
Tileset::Tileset() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
r.push_back(flyweight<std::string>(std::string("vertexShader")));
r.push_back(flyweight<std::string>(std::string("fragmentShader")));
r.push_back(flyweight<std::string>(std::string("tiles")));
}Tileset* toTileset(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Tileset")) {
  return static_cast<Tileset*>(b);
}
else {
printf("\"toTileset called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}

std::string Tileset::toJSON() const
{
nlohmann::json j {
{"Id", getId().get()},
{"Name", getName()},
{"Vertexshader", getVertexshader()},
{"Fragmentshader", getFragmentshader()}
};
return j.dump();
}
void Tileset::fromJSON(const char* json)
{

}

using nlohmann::json;

    void to_json(json& j, const Tileset& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Tileset& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Tileset* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void from_json(const json& j, Tileset* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
