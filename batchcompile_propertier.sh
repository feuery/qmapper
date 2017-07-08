source ./propertier.cfg
mkdir -p $output

./propertylisp/project/propertier-server --input-dir $input --output-dir $output --batch 1
