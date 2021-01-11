#!/bin/sh

data=$1
video=$2
transfert=$3

timestamp() {
  date +"%s" # current time
}

while [ true ]
do

psrecord $data --plot Data-$(timestamp).png --duration 1 &
psrecord $video --plot Video-$(timestamp).png --duration 1 &
psrecord $transfert --plot Transfert-$(timestamp).png --duration 1 

done