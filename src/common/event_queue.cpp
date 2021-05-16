#include "event_queue.h"

EventQueue* EventQueue::m_instance = NULL;
std::mutex EventQueue::mymutex;
