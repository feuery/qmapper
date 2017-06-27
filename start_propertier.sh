source ./propertier.cfg
mkdir $output

echo Starting propertier-compiler that reads data from $input to $output
./propertylisp/project/propertier-server --input-dir $input --output-dir $output
