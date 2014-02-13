// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include <map>
#include <pthread.h>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"

class lock_server {
  private:
  pthread_mutex_t& initLockAndGetRef(lock_protocol::lockid_t, int);
  void destroyLock(lock_protocol::lockid_t);

 protected:
  int nacquire;
  std::map < lock_protocol::lockid_t, pthread_mutex_t > locks;
  std::map < lock_protocol::lockid_t, unsigned int > count;
  pthread_mutex_t globalLock;

 public:
  lock_server();
  ~lock_server();
  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status acquire(int clt, lock_protocol::lockid_t lid, int &);
  lock_protocol::status release(int clt, lock_protocol::lockid_t lid, int &);
};

#endif 







