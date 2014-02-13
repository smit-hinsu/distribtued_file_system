// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

lock_server::lock_server():
  nacquire (0)
{
  pthread_mutex_init(&globalLock, NULL);
}

lock_server::~lock_server() 
{
  pthread_mutex_destroy(&globalLock);
  for(typeof(locks.begin()) it = locks.begin(); it != locks.end(); it++)
  {
    pthread_mutex_destroy(&it->second);
  }
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r)
{
  pthread_mutex_t &cur_lock = initLockAndGetRef(lid, 1);
  pthread_mutex_lock(&cur_lock);
  
  lock_protocol::status ret = lock_protocol::OK;
  return ret;
}

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
  pthread_mutex_t &cur_lock = initLockAndGetRef(lid, 0);
  pthread_mutex_unlock(&cur_lock);
  
  destroyLock(lid);
  lock_protocol::status ret = lock_protocol::OK;
  return ret;
}

pthread_mutex_t&
lock_server::initLockAndGetRef(lock_protocol::lockid_t lid, int inc) 
{
  //pthread_mutex_lock(&globalLock);
  int cur_count = count[lid];
  count[lid] += inc;
  if (cur_count == 0) 
  {
    pthread_mutex_init(&locks[lid], NULL);
    count[lid] = 1;
  }
  pthread_mutex_t &ret = locks[lid];
  //pthread_mutex_unlock(&globalLock);
  return ret;
}

void
lock_server::destroyLock(lock_protocol::lockid_t lid) 
{
  //pthread_mutex_lock(&globalLock);
  int new_count = (--count[lid]);
  if (new_count == 0) 
  {
    pthread_mutex_destroy(&locks[lid]);
  }
  //pthread_mutex_unlock(&globalLock);
}
