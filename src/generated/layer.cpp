#include <layer.h>
////// generated at 2018-01-18T13:33:15.993Z


void Layer::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Layer::getName() {
return Name_field;
}
void Layer::setOpacity(unsigned char value) { 
Opacity_field = value;
}
                                                        unsigned char Layer::getOpacity() {
return Opacity_field;
}
Layer::Layer() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
r.push_back(flyweight<std::string>(std::string("opacity")));
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
