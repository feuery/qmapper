#include <tool.h>

SCM getChosenMap()
{
  static SCM chosenMap = scm_c_lookup("root-chosenMap");
  return scm_call_1(chosenMap, editorController::instance->document);
}

bool Tool::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  static SCM map_w = scm_c_lookup("Map-width");
  static SCM map_h = scm_c_lookup("Map-height");
  SCM map = getChosenMap();
  if(scm_is_true(map) != 1) return false;
  
  return tilex < scm_to_int(scm_call_1(map_w, map)) &&
		 tiley < scm_to_int(scm_call_1(map_h, map)) &&
			 tilex >= 0 && tiley >= 0;
}

void Tool::mouseDown(QMouseEvent *e, editorController *ec) {
  static SCM chosenMap = scm_c_lookup("root-chosenMap");
  static SCM map_w = scm_c_lookup("Map-width");
  static SCM map_h = scm_c_lookup("Map-height");
  
  SCM map = scm_call_1(chosenMap, editorController::instance->document);
  if(scm_is_true(map) != 1) return;
    
  mouse_map = std::vector<std::vector<bool>>(scm_to_int(scm_call_1(map_w, map)),
					     std::vector<bool> (scm_to_int(scm_call_1(map_h, map)), false));
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
