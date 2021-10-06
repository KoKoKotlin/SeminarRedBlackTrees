#!/bin/sh

for file in $(find .);
do
    if [ ! -d $file ]; then
        if [ ${file##*.} = c -o ${file##*.} = h ]; then
            echo "Processing file $file:"
            python3 -m cpplint $file
            echo "\n"
        fi
    fi
done