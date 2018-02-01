#include <tile.h>
#include <json.hpp>
////// generated at 2018-02-01T09:49:48.757Z


void Tile::setX(int value) { 
X_field = value;
}
                                                        int Tile::getX() const {
return X_field;
}
void Tile::setY(int value) { 
Y_field = value;
}
                                                        int Tile::getY() const {
return Y_field;
}
void Tile::setRotation(int value) { 
Rotation_field = value;
}
                                                        int Tile::getRotation() const {
return Rotation_field;
}
void Tile::setTileset(std::string value) { 
Tileset_field = value;
}
                                                        std::string Tile::getTileset() const {
return Tileset_field;
}
Tile::Tile() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("x")));
r.push_back(flyweight<std::string>(std::string("y")));
r.push_back(flyweight<std::string>(std::string("tileset")));
r.push_back(flyweight<std::string>(std::string("rotation")));
}Tile* toTile(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Tile")) {
  return static_cast<Tile*>(b);
}
else {
printf("\"toTile called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}

std::string Tile::toJSON() const
{
nlohmann::json j {
{"Id", getId().get()},
{"X", getX()},
{"Y", getY()},
{"Tileset", getTileset()},
{"Rotation", getRotation()}
};
return j.dump();
}
void Tile::fromJSON(const char* json)
{

}

using nlohmann::json;

    void to_json(json& j, const Tile& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, Tile& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const Tile* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse("{\"error\": \"c is null\"}");
    }

    void from_json(const json& j, Tile* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
