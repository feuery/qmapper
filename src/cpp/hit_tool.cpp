#include <hit_tool.h>

bool Hit_tool::canUse(QMouseEvent *event, int tilex, int tiley, editorController *e) {
  return !toolAppliedTo(tilex, tiley) && tilex < e->chosenMapWidth() && tiley < e->chosenMapHeight();
}

void Hit_tool::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  cl_object invert_in_chosen_map = makefn("qmapper.map:invert-hit-tile-in-chosen-map"),
    x = ecl_make_int(tilex),
    y = ecl_make_int(tiley);
  e->document.setValue(cl_funcall(4, invert_in_chosen_map, e->document.getValue(), x, y));
}

bool Hit_tool::isHitTool() {
  return true;
}
