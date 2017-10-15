#include <texture.h>
////// generated at 2017-10-15T11:54:55.638Z


void Texture::setName(std::string val) {
Name_field = val;
}
                                                        std::string Texture::getName() {
return Name_field;
}
Texture::Texture() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("name")));
}Texture* toTexture(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Texture")) {
  return static_cast<Texture*>(b);
}
else {
printf("\"toTexture called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}
