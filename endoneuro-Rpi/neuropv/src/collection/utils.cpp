#include "utils.h"
#include <iostream>
#include <mutex>


std::mutex printLock;

void lockPrint(std::string msg) {
  std::lock_guard<std::mutex> guard(printLock);
  std::cout << msg;
}

void lockPrintln(std::string msg) {
  std::lock_guard<std::mutex> guard(printLock);
  std::cout << msg << std::endl;
}