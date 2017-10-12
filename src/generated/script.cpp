#include <script.h>
////// generated at 2017-10-12T15:52:37.555Z


void Script::setContents(std::string val) {
Contents_field = val;
}
                                                        std::string Script::getContents() {
return Contents_field;
}
void Script::setName(std::string val) {
Name_field = val;
}
                                                        std::string Script::getName() {
return Name_field;
}
void Script::setNs(std::string val) {
Ns_field = val;
}
                                                        std::string Script::getNs() {
return Ns_field;
}
void Script::setScript_type(scriptTypes val) {
Script_type_field = val;
}
                                                        scriptTypes Script::getScript_type() {
return Script_type_field;
}
Script::Script() {
r.push_back(flyweight<std::string>(std::string("Id")));
r.push_back(flyweight<std::string>(std::string("contents")));
r.push_back(flyweight<std::string>(std::string("name")));
r.push_back(flyweight<std::string>(std::string("ns")));
r.push_back(flyweight<std::string>(std::string("script_type")));
}Script* toScript(Propertierbase *b)
 {
if(b->type_identifier() == std::string("Script")) {
  return static_cast<Script*>(b);
}
else {
printf("\"toScript called with \"%s\"\n", b->type_identifier().get().c_str());
throw "";
}
}
