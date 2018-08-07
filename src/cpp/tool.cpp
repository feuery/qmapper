#include <tool.h>

cl_object getChosenMap()
{
  static cl_object chosenMap = ecl_make_symbol("root-chosenMap", "CL-USER");
  return cl_funcall(2, chosenMap, editorController::instance->document);
}

bool Tool::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  static cl_object map_w = ecl_make_symbol("Map-width", "CL-USER");
  static cl_object map_h = ecl_make_symbol("Map-height", "CL-USER");
  cl_object map = getChosenMap();
  if(!Null(map)) return false;
  
  return tilex < fixint(cl_funcall(2, map_w, map)) &&
		 tiley < fixint(cl_funcall(2, map_h, map)) &&
			 tilex >= 0 && tiley >= 0;
}

void Tool::mouseDown(QMouseEvent *e, editorController *ec) {
  static cl_object chosenMap = ecl_make_symbol("root-chosenMap", "CL-USER");
  static cl_object map_w = ecl_make_symbol("Map-width", "CL-USER");
  static cl_object map_h = ecl_make_symbol("Map-height", "CL-USER");
  
  cl_object map = cl_funcall(2, chosenMap, editorController::instance->document);
  if(!Null(map)) return;
    
  mouse_map = std::vector<std::vector<bool>>(fixint(cl_funcall(2, map_w, map)),
					     std::vector<bool> (fixint(cl_funcall(2, map_h, map)), false));
}

void Tool::mouseUp(QMouseEvent *e) { }

bool Tool::toolAppliedTo(int tilex, int tiley) {
  return mouse_map.at(tilex).at(tiley);
}

void Tool::registerDrag(int tilex, int tiley) {
  if(mouse_map.size() == 0) return;

  if(!canUse(nullptr, tilex, tiley, editorController::instance)) return;
  
  mouse_map.at(tilex).at(tiley) = true;
}
