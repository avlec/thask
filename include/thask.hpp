#ifndef THASK_HPP_
#define THASK_HPP_

#include <tuple>
#include <functional>

#define THASK_THREAD_MODE

#ifdef THASK_THREAD_MODE
#include <thread>
#elif defined(THASK_TASK_MODE)

#endif

struct thask_config
{
    enum class alloc_type : uint8_t
    {
        normal, staticNormal, staticRestricted
    };

    const char* name;
    // FreeRTOS Specific
    uint32_t stackDepth;
    // FreeRTOS Specific [0, configMAX_PRIORITIES) where configMAX_PRIORITIES < 32
    uint8_t priority;

    alloc_type allocType;

    // StackType_t*
    void* stackBuffer;
    // StaticTask_t*
    void* taskBuffer;
};

template <class... Args>
struct thask
{
    using handle_t = std::thread;

    using args_t = std::tuple<Args...>;

    handle_t handle;
    args_t   args;

    thask(thask_config& tc, std::function<void(void*)>&& fn, Args&&... a)
    {
#ifdef THASK_THREAD_MODE
        this->args   = std::make_tuple(a...);
        this->handle = std::thread{fn, &this->args};

        pthread_setname_np(this->handle.native_handle(), tc.name);
#elif defined(THASK_TASK_MODE)
        switch(tc.allocType)
        {
            case thaskthask_config::alloc_type::normal:
                xTaskCreate(fn, tc.name, tc.stackDepth, &this->args, tc.priority, &this->handle);
                break;
            case thaskthask_config::alloc_type::staticNormal:
                this->handle = xTaskCreateStatic();
                break;
            case thaskthask_config::alloc_type::staticRestricted:
                TaskParameters_t tp;
                xTaskCreateRestrictedStatic(&tp, &this->handle);
                break;

        }
#endif
    }

    ~thask()
    {
#ifdef THASK_THREAD_MODE
        this->handle.join();
#elif defined(THASK_TASK_MODE)
#endif
    }
};

#endif // THASK_HPP_
