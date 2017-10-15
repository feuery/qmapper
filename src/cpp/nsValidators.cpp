#include <nsValidators.h>
#include <editorController.h>

bool doesntContainNs(std::string ns) {
  return !editorController::instance->document.containsNs(ns);
}
