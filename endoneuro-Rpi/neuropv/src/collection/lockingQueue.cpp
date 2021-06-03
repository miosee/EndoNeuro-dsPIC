#include "lockingQueue.h"


void LockingQueue::push(uint16_t value) {
  std::lock_guard<std::mutex> guard(lock);
  q.push(value);
}

void LockingQueue::pop() {
  std::lock_guard<std::mutex> guard(lock);
  q.pop();
}

uint16_t LockingQueue::front() {
  std::lock_guard<std::mutex> guard(lock);
  return ( q.front() );
}

bool LockingQueue::empty() {
  std::lock_guard<std::mutex> guard(lock);
  return ( q.empty() );
}

uint32_t LockingQueue::size() {
  std::lock_guard<std::mutex> guard(lock);
  return ( q.size() );
}