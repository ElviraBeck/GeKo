#pragma once
#include <cstdint>
#include <vector>
#include <stack>
#include "GeKo_Graphics/Geometry/Geometry.h"
namespace Handle{
  template<class T>
    struct HandleManager;
  using HandleID = uint16_t;
  using Counter = uint8_t;

  template<class T>
    struct Handle{
      T get(){
        return manager->get(*this);
      }

      HandleManager<T> *manager;
      HandleID id;
      Counter counter;
    };
  template<class T>
    struct HandleManager{
      std::vector<T> data;
      std::vector<Counter> counter;
      std::stack<HandleID> freeList;

      Handle<T> add(T t){
        if(freeList.size() > 0){
          HandleID nextFreeHandleId = freeList.top();
          data[nextFreeHandleId] = t;
          auto h = Handle<T>{this,nextFreeHandleId, counter[nextFreeHandleId]};
          freeList.pop();
          return h;
        }
        else{
          data.push_back(t);
          counter.push_back(0);
          HandleID id = data.size() -1;
          return Handle<T>{this,id, counter[id]};
        }
      }

      void remove(Handle<T> handle){
        freeList.push(handle.id);
        counter[handle.id] += 1;
      }

      T get(Handle<T> handle){
        if(!validate(handle)){
          throw "Err";
        }
        return data[handle.id];
      }
      bool validate(Handle<T> handle){
        return handle.counter == counter[handle.id];
      }
    };
}


