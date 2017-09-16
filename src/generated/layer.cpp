#include <layer.h>
////// generated at 2017-09-16T10:57:44.767Z

void Layer::setName(std::string val) {
Name_field = val;
}
                                                        std::string Layer::getName() {
return Name_field;
}
Layer::Layer() {
r[0] = "name";
}