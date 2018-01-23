#!/bin/env bash

# run this to generate the rudimentary description of the tools available compile-time

printf "#include<unordered_map>\n#include<string>\n#include<tool.h>\nextern std::unordered_map<std::string, Tool*> tool_cache;" > ./src/headers/tool_list.h
printf "#include<tool_list.h>\n" > ./src/cpp/tool_list.cpp

grep ': public Tool' ./src/headers/*.h | while read -r line; do
    file_path=$( echo "$line" | sed -E 's/^.+\/(.+).h.+$/\1/g')
    printf "#include<$file_path.h>\n" >> ./src/cpp/tool_list.cpp
done

printf "std::unordered_map<std::string, Tool*> tool_cache {" >> ./src/cpp/tool_list.cpp

grep ': public Tool' ./src/headers/*.h | while read -r line; do
    class_name=$(echo "$line" | sed -E 's/^.+class (.+): .+$/\1/g')
    printf "{\"$class_name\", new $class_name},\n" >> ./src/cpp/tool_list.cpp
done
    
echo '};' >> ./src/cpp/tool_list.cpp
