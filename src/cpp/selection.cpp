#include <selection.h>

void SelectionTool::mouseUp(QMouseEvent *e)
{
  // puts("mouseup");
  // cl_object select_tile = makefn("qmapper.root:select-tile"),
  //   doc = editorController::instance->document.getValue(),
  //   new_doc = cl_funcall(4, select_tile, doc, ECL_NIL, ECL_NIL);
  // puts("mouseupper");
  // editorController::instance->document.setValue(new_doc);
}

void SelectionTool::use(QMouseEvent *event, int tilex, int tiley, editorController *e)
{
  cl_object doc = e->document.getValue();
  assert(doc != ECL_NIL);
  cl_object select_tile = makefn("qmapper.root:select-tile"),
    get_selection = makefn("qmapper.root:root-selected-coordinates"),
    format = makefn("format");
  cl_object cl_x = ecl_make_int32_t(tilex),
    cl_y = ecl_make_int32_t(tiley),
    new_doc = cl_funcall(4, select_tile, doc, cl_x, cl_y);
  assert(new_doc != ECL_NIL);
  cl_funcall(4, format, ECL_T, c_string_to_object("\"selection at use ~a~%\""), cl_funcall(2, get_selection, new_doc));
  e->document.setValue(new_doc);
}

void SelectionTool::mouseDown(QMouseEvent *event, editorController *e)
{
  cl_object doc = e->document.getValue(),
    format = makefn("format");

  cl_object select_tile = makefn("qmapper.root:select-tile"),
    reset_selection = makefn("qmapper.root:reset-selection"),
    new_doc = cl_funcall(2, reset_selection, doc),
    get_selection = makefn("qmapper.root:root-selected-coordinates");

  new_doc = cl_funcall(4, select_tile, new_doc, ecl_make_int32_t(0), ecl_make_int32_t(0));
  assert(new_doc != ECL_NIL);
  cl_funcall(4, format, ECL_T, c_string_to_object("\"selection at mousedown ~a~%\""), cl_funcall(2, get_selection, new_doc));
  e->document.setValue(new_doc);
}
