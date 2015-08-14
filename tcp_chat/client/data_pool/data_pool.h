#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include <semaphore.h>
#include <vector>
#include <iostream>
#include "util.h"
using namespace std;

<<<<<<< HEAD
template<typename _value_type>
=======
template<typename value_type>
>>>>>>> branch
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

<<<<<<< HEAD
		int push(const _value_type value)
		{
			data_pool::sem_p(&_empty);
=======
		int push(const value_type value)
		{
			if(data_pool::sem_p(&_empty) == -1){
				return -1;
			}
>>>>>>> branch
			_msg_pool[_head] = value;
			data_pool::sem_v(&_full);
			_head = (_head + 1)%MAX_SLOT;
			
			return 0;
		}
<<<<<<< HEAD
		_value_type pop()
		{
			data_pool::sem_p(&_full);
			_value_type data = _msg_pool[_tail];
			data_pool::sem_v(&_empty);
			_tail = (_tail + 1)%MAX_SLOT;

			return data;
		
		}

		_value_type &head()
		{
			return _msg_pool[(_head -1)%MAX_SLOT];
		}
		_value_type &tail()
=======
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
>>>>>>> branch
		{
			return _msg_pool[_tail];
		}

	private:
		inline static int sem_p(sem_t *sem)
		{
<<<<<<< HEAD
			return sem_wait(sem);
=======
			return sem_trywait(sem);
>>>>>>> branch
		}
		inline static int sem_v(sem_t *sem)
		{
			return sem_post(sem);
		}


	private:

<<<<<<< HEAD
		vector<_value_type>	_msg_pool;
=======
		vector<value_type>	_msg_pool;
>>>>>>> branch
		int					_head;
		int					_tail;
		sem_t				_full;
		sem_t				_empty;
};



#endif
