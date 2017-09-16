#include <map.h>
////// generated at 2017-09-16T12:30:53.300Z

void Map::setName(std::string val) {
Name_field = val;
}
                                                        std::string Map::getName() {
return Name_field;
}
Map::Map() {
r[0] = "name";
}