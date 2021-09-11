#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <functional>
#include <type_traits>
#include <future>
#include <memory>
#include <algorithm>

#include <functional>
#include <type_traits>
#include <future>
#include <memory>
#include <algorithm>

namespace csugl {

	class thread_pool {
	public:
		explicit thread_pool(size_t size)
			:isDestroy(false) {
			for (size_t i = 0; i < size; i++)
			{
				_threads.emplace_back([this]
					{
						while (true)
						{
							std::function<void()> task;
							{
								//  Get lock
								std::unique_lock<std::mutex> lock(_mutex);
								//  Wait task
								_condition.wait(lock, [this]()
									{
										return this->isDestroy || !this->_tasks.empty();
									});
								//  Destroy thread
								if (this->isDestroy && this->_tasks.empty())
								{
									return;
								}
								//  Get task
								task = std::move(this->_tasks.front());
								this->_tasks.pop();
							}
							//  Do task
							task();
						}
					});
			}
		}

		~thread_pool() {
			{
				std::unique_lock<std::mutex> lock(_mutex);
				isDestroy = true;
			}
			//  Notify all
			_condition.notify_all();
			// Join all
			for_each(_threads.begin(), _threads.end(), std::mem_fn(&std::thread::join));
		}

		template<class F, class ... Args>
		decltype(auto) submit(F&& f, Args&&... args);

		inline size_t getThreadSize() const {
			return _threads.size();
		}

		inline size_t getTaskSize() const {
			return _tasks.size();
		}

	private:
		std::vector<std::thread> _threads;
		std::queue<std::function<void()>> _tasks;
		mutable std::mutex _mutex;
		std::condition_variable _condition;

		bool isDestroy;
	};

	template<class F, class ...Args>
	inline decltype(auto) thread_pool::submit(F&& f, Args && ...args) {
		using return_type = std::invoke_result_t<F, Args...>;

		auto task = std::make_shared< std::packaged_task<return_type()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

		std::future<return_type> result = task->get_future();

		{
			std::unique_lock<std::mutex> lock(_mutex);
			if (isDestroy)
			{
				throw std::runtime_error("Add task on detroyed thread pool");
			}

			//  add task
			_tasks.emplace([task]() { (*task)(); });
		}
		_condition.notify_one();
		return result;
	}

}
