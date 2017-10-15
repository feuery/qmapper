#include <map.h>
////// generated at 2017-10-15T11:54:55.650Z


void Map::setName(std::string val) {
Name_field = val;
}
                                                        std::string Map::getName() {
return Name_field;
}
void Map::setText(Texture* val) {
Text_field = val;
}
                                                        Texture* Map::getText() {
return Text_field;
}
Map::Map() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
r.push_back(flyweight<std::string>(std::string("text")));
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
