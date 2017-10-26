#include <script.h>
////// generated at 2017-10-26T16:03:55.370Z


void Script::setContents(std::string value) { 
Contents_field = value;
}
                                                        std::string Script::getContents() {
return Contents_field;
}
void Script::setName(std::string value) { 
Name_field = value;
}
                                                        std::string Script::getName() {
return Name_field;
}
void Script::setNs(std::string value) { 
auto error_reporter = [&](std::string msg) {
    std::vector<std::string>& vec = field_error_map[std::string("Ns")];
	printf("Pushing error %s\n", msg.c_str());
	vec.push_back(msg);};if(doesntContainNs(value, error_reporter))Ns_field = value;
}
                                                        std::string Script::getNs() {
return Ns_field;
}
void Script::setScript_type(scriptTypes value) { 
Script_type_field = value;
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
