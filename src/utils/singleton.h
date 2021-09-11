#pragma once

#include <mutex>
#include <memory>

namespace csugl {

	template<typename T>
	class singleton {
	public:
		template<typename ...Args>
		static std::shared_ptr<T> getInstance(Args&&... args) {
			if (!instance) {
				std::lock_guard<std::mutex> gLock(_mutex);
				if (nullptr == instance) {
					instance = std::make_shared<T>(std::forward<Args>(args)...);
				}
			}
			return instance;
		}

		static void destroy() {
			if (instance) {
				instance.reset();
				instance = nullptr;
			}
		}

    public:
		explicit singleton() = delete;
		singleton(const singleton&) = delete;
		singleton& operator=(const singleton&) = delete;
		//~Singleton();

	private:
		static std::shared_ptr<T> instance;
		static std::mutex _mutex;
	};

	template<typename T>
	std::shared_ptr<T> singleton<T>::instance = nullptr;

	template<typename T>
	std::mutex singleton<T>::_mutex;

}