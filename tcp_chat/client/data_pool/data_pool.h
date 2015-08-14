#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include <semaphore.h>
#include <vector>
#include <iostream>
#include "util.h"
using namespace std;

template<typename value_type>
class data_pool {
	public:
		enum{MAX_SLOT = 10};
		
		data_pool()
			: _head(0), _tail(0)
		{
			sem_init(&_full, 0, 0);
			sem_init(&_empty, 0, MAX_SLOT);
			_msg_pool.resize(MAX_SLOT);
		}
		~data_pool()
		{
			sem_destroy(&_full);
			sem_destroy(&_empty);
		}

		int push(const value_type value)
		{
			if(data_pool::sem_p(&_empty) == -1){
				return -1;
			}
			_msg_pool[_head] = value;
			data_pool::sem_v(&_full);
			_head = (_head + 1)%MAX_SLOT;
			
			return 0;
		}
		int pop(value_type &value)
		{
			if(data_pool::sem_p(&_full) == -1){
				return -1;
			}
			value = _msg_pool[_tail];
			data_pool::sem_v(&_empty);
			_tail = (_tail + 1)%MAX_SLOT;

			return 0;
		
		}

		value_type &head()
		{
			return _msg_pool[(_head -1)%MAX_SLOT];
		}
		value_type &tail()
		{
			return _msg_pool[_tail];
		}

	private:
		inline static int sem_p(sem_t *sem)
		{
			return sem_trywait(sem);
		}
		inline static int sem_v(sem_t *sem)
		{
			return sem_post(sem);
		}


	private:

		vector<value_type>	_msg_pool;
		int					_head;
		int					_tail;
		sem_t				_full;
		sem_t				_empty;
};



#endif
