# EndoNeuro

EndoNeuro system acquisition application

## Building and Running

### On The RPI

To build copy the project and at the root of the project:

```sh
sh scripts/build.sh
```

To run, at the root of the project:

```sh
sh scripts/run.sh
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release
```

## App explanation

This app is divided in Three part :

- Data collection
- Video Collection
- Transfert Process

Data collection allow us to made the acquisition from EEG and save it on a file .csv
Video capture from RPI camera and save it on a file
Transfert process move each file to an another directory

# Tools

## Monitoring

To compare the performance before and after the change we needed to get the use of each part ( Video, data, transfert).
For that we use an independant application psrecord
To make a plot of the activity:

psrecord PID --plot plot.png

## To monitor us 3 processus we create an bash script

```sh
sh tools/monith.sh DATAPID1 VIDEOPID2 TRANSFERTPID3
```

The monitoring make an picture with CPU and memory usage while 1 second.

## Check file

After collecting data we need to check if their contents have no trouble.
The data value increases from 0-255 with that is easy to know if trouble comes.

- Edit : 
```sh
nano tools/file_checker.py
```
count : TimeStamp of Datafile;
check_var : First value of your first data file.

- Launch :
```sh
python file_checker.py
```
