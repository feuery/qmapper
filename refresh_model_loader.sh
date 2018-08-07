#!/bin/bash

echo "#include <loader.h>" > src/scm_model/loader.cpp
echo "std::vector<std::string> models {" >> src/scm_model/loader.cpp

ls src/scm_model/*.lisp |while read -r fname; do
    fname=$(echo $fname| sed -E 's!src/scm_model/(.+)\.lisp!\1!g')
    echo "\"$fname\"," >> src/scm_model/loader.cpp 
done;

echo "};" >> src/scm_model/loader.cpp
