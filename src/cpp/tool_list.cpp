#include<tool_list.h>
#include<pen.h>
#include<rotater.h>
#include<spriteMover.h>
std::unordered_map<std::string, Tool*> tool_cache {{"Pen", new Pen},
{"Rotater", new Rotater},
{"Spritemover", new Spritemover},
};
