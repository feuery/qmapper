#include <root.h>
////// generated at 2017-09-26T17:32:11.382Z


root::root() {
r.push_back(flyweight<std::string>(std::string("Id")));
}root* toRoot(Propertierbase *b)
 {
if(b->type_identifier() == std::string("root")) {
  return static_cast<root*>(b);
}
else {
printf("\"toRoot called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}
