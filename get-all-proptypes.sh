source ./propertier.cfg

if [ ! -f ./clj-propertylisp/target/clj-propertylisp-0.1.0-SNAPSHOT-standalone.jar ]; then
    pushd clj-propertylisp
    lein uberjar
    popd
fi

java -jar ./clj-propertylisp/target/clj-propertylisp-0.1.0-SNAPSHOT-standalone.jar --get-every-prop-type $input
