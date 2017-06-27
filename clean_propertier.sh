source ./propertier.cfg

if [ ! -d $output ]; then
    echo "Output directory not found. Is \$output configured correctly as \"$output\"?"
    echo "If not, please config output directory to the ./propertier.config"
    exit
else
    echo "This cleans the propertier result by running \"rm -rf $output\""
    echo "If this is okay, press return"

    read -p "else Ctrl+C" res

    if [ ${#var} -eq 0 ]; then
	rm -rf $output
    fi
fi
