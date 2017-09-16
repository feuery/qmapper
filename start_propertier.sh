source ./propertier.cfg
mkdir -p $output

if [ ! -f ./clj-propertylisp/target/clj-propertylisp-0.1.0-SNAPSHOT-standalone.jar ]; then
    pushd clj-propertylisp
    lein uberjar
    popd
fi

# echo Starting propertier-compiler that reads data from $input to $output
java -jar ./clj-propertylisp/target/clj-propertylisp-0.1.0-SNAPSHOT-standalone.jar $input $output
