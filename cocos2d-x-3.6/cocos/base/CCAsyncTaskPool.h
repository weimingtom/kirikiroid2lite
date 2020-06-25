/****************************************************************************
Copyright (c) 2013-2015 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCSYNC_TASK_POOL_H_
#define __CCSYNC_TASK_POOL_H_

#include "platform/CCPlatformMacros.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include <vector>
#include <queue>
#include <memory>
#include <pthread.h>
#include <functional>
#include <stdexcept>

/**
* @addtogroup base
* @{
*/
NS_CC_BEGIN


/**
 * @class AsyncTaskPool
 * @brief This class allows to perform background operations without having to manipulate threads.
 * @js NA
 */
class CC_DLL AsyncTaskPool
{
public:
    typedef std::function<void(void*)> TaskCallBack;
    
    enum TaskType
    {
        TASK_IO,
        TASK_NETWORK,
        TASK_OTHER,
        TASK_MAX_TYPE,
    };

    /**
     * Returns the shared instance of the async task pool.
     */
    static AsyncTaskPool* getInstance();

    /**
     * Destroys the async task pool.
     */
    static void destoryInstance();
    
    /**
     * Stop tasks.
     *
     * @param type Task type you want to stop.
     */
    void stopTasks(TaskType type);
    
    /**
     * Enqueue a asynchronous task.
     *
     * @param type task type is io task, network task or others, each type of task has a thread to deal with it.
     * @param callback callback when the task is finished. The callback is called in the main thread instead of task thread.
     * @param callbackParam parameter used by the callback.
     * @param f task can be lambda function.
     * @lua NA
     */
    template<class F>
    inline void enqueue(TaskType type, const TaskCallBack& callback, void* callbackParam, F&& f);
    
CC_CONSTRUCTOR_ACCESS:
    AsyncTaskPool();
    ~AsyncTaskPool();
    
protected:
    
    // thread tasks internally used
    class ThreadTasks {
        struct AsyncTaskCallBack
        {
            TaskCallBack          callback;
            void*                 callbackParam;
        };
    public:
        ThreadTasks()
        : _stop(false)
        {
#if defined(ANDROID)
			__android_log_print(ANDROID_LOG_ERROR, "CCAsyncTaskPool.h", "%s", "*** *** ThreadTasks");
#endif
			pthread_mutex_init(&_queueMutex, NULL);
			pthread_cond_init(&_condition, NULL);
            pthread_create(&_thread, NULL, &ThreadTasks::loadData_entry, this);
        }
		static void* loadData_entry(void *pthis) {
		  ThreadTasks *obj = static_cast<ThreadTasks *>(pthis);
		  obj->loadData();
		  return NULL;
		}
		void loadData()
		{
			for(;;)
            {
                std::function<void()> task;
                AsyncTaskCallBack callback;
                {
                    pthread_mutex_lock(&this->_queueMutex);
					while (true) {
						//see https://zh.cppreference.com/w/cpp/thread/condition_variable/wait
						if (this->_stop || !this->_tasks.empty()) break;
						pthread_cond_wait(&this->_condition, &this->_queueMutex);
					}
                    if(this->_stop && this->_tasks.empty()) {
                        pthread_mutex_unlock(&this->_queueMutex);
						return;
					}
                    task = std::move(this->_tasks.front());
                    callback = std::move(this->_taskCallBacks.front());
                    this->_tasks.pop();
                    this->_taskCallBacks.pop();
					pthread_mutex_unlock(&this->_queueMutex);
				}                    
                task();
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, callback]{ callback.callback(callback.callbackParam); });
            }
		}
        ~ThreadTasks()
        {
            {
                pthread_mutex_lock(&_queueMutex);
                _stop = true;
                
                while(_tasks.size())
                    _tasks.pop();
                while (_taskCallBacks.size())
                    _taskCallBacks.pop();
				pthread_mutex_unlock(&_queueMutex);            
            }
            pthread_cond_signal(&_condition);
            pthread_join(_thread, NULL);
        }
        void clear()
        {
            pthread_mutex_lock(&_queueMutex);
            while(_tasks.size())
                _tasks.pop();
            while (_taskCallBacks.size())
                _taskCallBacks.pop();
			pthread_mutex_unlock(&_queueMutex);
        }
        template<class F>
        void enqueue(const TaskCallBack& callback, void* callbackParam, F&& f)
        {
            auto task = f;//std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            
            {
                pthread_mutex_lock(&_queueMutex);
                
                // don't allow enqueueing after stopping the pool
                if(_stop)
                {
                    pthread_mutex_unlock(&_queueMutex);
					CC_ASSERT(0 && "already stop");
					return;
                }
                
                AsyncTaskCallBack taskCallBack;
                taskCallBack.callback = callback;
                taskCallBack.callbackParam = callbackParam;
                _tasks.push([task](){ task(); });
                _taskCallBacks.push(taskCallBack);
				pthread_mutex_unlock(&_queueMutex);
            }
            pthread_cond_signal(&_condition);
        }
    private:
        
        // need to keep track of thread so we can join them
        pthread_t _thread;
        // the task queue
        std::queue< std::function<void()> > _tasks;
        std::queue<AsyncTaskCallBack>            _taskCallBacks;
        
        // synchronization
        pthread_mutex_t _queueMutex;
        pthread_cond_t _condition;
        bool _stop;
    };
    
    //tasks
    ThreadTasks _threadTasks[int(TaskType::TASK_MAX_TYPE)];
    
    static AsyncTaskPool* s_asyncTaskPool;
};

inline void AsyncTaskPool::stopTasks(TaskType type)
{
    auto& threadTask = _threadTasks[(int)type];
    threadTask.clear();
}

template<class F>
inline void AsyncTaskPool::enqueue(AsyncTaskPool::TaskType type, const TaskCallBack& callback, void* callbackParam, F&& f)
{
    auto& threadTask = _threadTasks[(int)type];
    
    threadTask.enqueue(callback, callbackParam, f);
}


NS_CC_END
// end group
/// @}
#endif //__CCSYNC_TASK_POOL_H_
