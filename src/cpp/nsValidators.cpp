#include <nsValidators.h>

#include <editorController.h>

bool doesntContainNs(std::string ns, const std::function<void(std::string)> &error_reporter) {
  if(!editorController::instance->document.containsNs(ns)) return true;
  std::string report = std::string("There's already a script with ns ") + ns;
  printf("Reporting %s\n", report.c_str());
  error_reporter(report);
  return false;
}


