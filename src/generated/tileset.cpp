#include <tileset.h>
////// generated at 2018-01-31T14:08:33.503Z


void Tileset::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Tileset::getName() {
return Name_field;
}
void Tileset::setVertexshader(Script* value) { 
Vertexshader_field = value;
}
                                                        Script* Tileset::getVertexshader() {
return Vertexshader_field;
}
void Tileset::setFragmentshader(Script* value) { 
Fragmentshader_field = value;
}
                                                        Script* Tileset::getFragmentshader() {
return Fragmentshader_field;
}
void Tileset::setTiles(std::vector<std::vector<Tile*>>* value) { 
Tiles_field = value;
}
                                                        std::vector<std::vector<Tile*>>* Tileset::getTiles() {
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
