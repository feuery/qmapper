#include <layer.h>
////// generated at 2017-10-28T17:12:10.929Z


void Layer::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Layer::getName() {
return Name_field;
}
Layer::Layer() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
}Layer* toLayer(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Layer")) {
  return static_cast<Layer*>(b);
}
else {
printf("\"toLayer called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}
