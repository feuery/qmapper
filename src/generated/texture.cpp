#include <texture.h>
////// generated at 2018-01-17T18:45:43.722Z


void Texture::setName(std::string value) { 
Name_field = value;
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
