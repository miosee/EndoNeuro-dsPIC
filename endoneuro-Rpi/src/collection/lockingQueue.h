#ifndef LOCKINGQUEUE_H // include guard
#define LOCKINGQUEUE_H


#include <mutex>
#include <queue>

class LockingQueue {
  private: 
    std::queue<uint16_t> q;
    std::mutex lock;

  public:
    void push(uint16_t value);
    void pop();
    uint16_t front();
    bool empty();
    uint32_t size();
};


#endif