#include <map.h>
////// generated at 2017-09-16T10:57:44.767Z

void Map::setName(std::string val) {
Name_field = val;
}
                                                        std::string Map::getName() {
return Name_field;
}
Map::Map() {
r[0] = "name";
}