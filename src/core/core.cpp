//
// Created by 匡涂上青 on 2021/6/18.
//

#include <memory>
#include "core.h"

// stbi init
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#endif


#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>

#endif


#if WIN32

#include "windows.h"

#else

#include "unistd.h"

#endif

namespace csugl {

    unsigned int get_core_numbers() {
#if WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        int allCPUNum_ = sysInfo.dwNumberOfProcessors;
        return allCPUNum_;
#else //linux
        //获取当前系统的所有CPU核数，包含禁用的
        int allCPUNum_ = sysconf(_SC_NPROCESSORS_CONF);
        //获取当前系统的可用CPU核数
        int enableCPUNum_ = sysconf(_SC_NPROCESSORS_ONLN);
        return enableCPUNum_;
#endif
    }

}