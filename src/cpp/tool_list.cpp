#include<tool_list.h>
#include<hit_tool.h>
#include<pen.h>
#include<rotater.h>
#include<selection.h>
#include<spriteMover.h>
#include<spriteRotater.h>
std::unordered_map<std::string, Tool*> tool_cache {{"Hit_tool", new Hit_tool},
{"Pen", new Pen},
{"Rotater", new Rotater},
{"SelectionTool", new SelectionTool},
{"Spritemover", new Spritemover},
{"Spriterotater", new Spriterotater},
};
