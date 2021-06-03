#!/bin/bash

if [ "$1" = "release" ]
then
    ./bin/release/EndoNeuro 
else
    ./bin/debug/EndoNeuro 
fi