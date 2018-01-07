#include <map.h>
////// generated at 2018-01-07T16:54:10.890Z


void Map::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Map::getName() {
return Name_field;
}
void Map::setText(Texture* value) { 
Text_field = value;
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
