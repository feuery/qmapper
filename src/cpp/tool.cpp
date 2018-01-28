#include <tool.h>
#include <map.h>

bool Tool::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  if(e->indexOfChosenMap == "") return false;
  
  Map *m = toMap(e->document.fetchRegister("Map", e->indexOfChosenMap));
  return tilex < m->width() && tiley < m->height() && tilex >= 0 && tiley >= 0;
}

void Tool::mouseDown(editorController *e) {
  if(e->indexOfChosenMap == "") return;
  
  Map *m = toMap(e->document.fetchRegister("Map", e->indexOfChosenMap));
  mouse_map = std::vector<std::vector<bool>>(m->width(),
					     std::vector<bool> (m->height(), false));
}

bool Tool::toolAppliedTo(int tilex, int tiley) {
  return mouse_map.at(tilex).at(tiley);
}

void Tool::registerDrag(int tilex, int tiley) {
  if(mouse_map.size() == 0) return;

  if(!canUse(nullptr, tilex, tiley, editorController::instance)) return;
  
  mouse_map.at(tilex).at(tiley) = true;
}
