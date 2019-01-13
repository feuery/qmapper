#include <tool.h>
#include <QString>

cl_object getChosenMap()
{
  cl_object chosenMap = makefn("qmapper.root:root-chosenMap"),
    maps = makefn("qmapper.root:root-maps"),
    format = makefn("format");
  cl_object root = editorController::instance->document.getValue();
  std::string chosenMapid = ecl_string_to_string(cl_funcall(2, chosenMap,root));
  QString qchosenMapid = chosenMapid.c_str();
  // QString::replace is thousand times easier to use than std::string::replace
  // IMO worth the data-type-juggling at this point of development
  // this looks like nice low hanging optimization fruit for someone under influence of enough booze to use stl's classes
  qchosenMapid.replace("\"", "");
  printf("chosenMap is %s\n", qchosenMapid.toStdString().c_str());
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"maps are ~a~%\""), cl_funcall(2, maps, root));
  
  return get(cl_funcall(2, maps, root), qchosenMapid.toStdString().c_str());
}

bool Tool::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  cl_object map_w = makefn("qmapper.map:Map-Width");
  cl_object map_h = makefn("qmapper.map:Map-height");
  cl_object map = getChosenMap();
  if(map == ECL_NIL) { puts("can't use tool, map is ECL_NIL"); return false; }
  
  return tilex < fixint(cl_funcall(2, map_w, map)) &&
		 tiley < fixint(cl_funcall(2, map_h, map)) &&
			 tilex >= 0 && tiley >= 0;
}

void Tool::mouseDown(QMouseEvent *e, editorController *ec) {
  cl_object chosenMap = makefn("qmapper.root:root-chosenMap"),
    drop_keys = makefn("qmapper.std:drop-alist-keys"),
    maps = makefn("qmapper.root:root-maps");
  cl_object map_w = makefn("qmapper.map:Map-width");
  cl_object map_h = makefn("qmapper.map:Map-height"),
    nth = makefn("nth");
  cl_object root = ec->document.getValue(),
    map = get(root, ecl_string_to_string(cl_funcall(2, chosenMap, root)).c_str()),
    format = makefn("format");
  
  if(map == ECL_NIL) {
    puts("selected map is nil");
    return;
  }

  puts("Getting dimensions");
  cl_funcall(4, format, ECL_T, c_string_to_object("\"map is ~a~%\""), map);
  int map_width = fixint(cl_funcall(2, map_w, map));
  puts("We have map_w");
  int map_height = fixint(cl_funcall(2, map_h, map));
  puts("We have map_h");
    
  mouse_map = std::vector<std::vector<bool>>(map_width,
					     std::vector<bool> (map_height, false));
}

void Tool::mouseUp(QMouseEvent *e) { }

bool Tool::toolAppliedTo(int tilex, int tiley) {
  return mouse_map.at(tilex).at(tiley);
}

void Tool::registerDrag(int tilex, int tiley) {
  puts("registered_drag");
  printf("mousemap size %d\n", mouse_map.size());
  if(mouse_map.size() == 0) return;

  if(!canUse(nullptr, tilex, tiley, editorController::instance)) {
    puts("can't use");
    return;
  }
  
  mouse_map.at(tilex).at(tiley) = true;
}
