#pragma once

#include <vector>
#include <queue>
#include <atomic>
#include <thread>
#include <functional>
#include <type_traits>
#include <utility>
#include <future>
#include <stdexcept>

namespace atpdxy{

// CPU密集型线程数量
const size_t THREAD_CPU_TASK = 5;
// I/O密集型线程数量
const size_t THREAD_ID_TASK = 9;
// 是否自动增长
#define AUTO_GROW

class ThreadPool{
public:
    // 任务函数指针
    typedef std::function<void()> Task;

    explicit ThreadPool(size_t n)
        :thread_nums(n)
    {
        if(n==0){
            thread_nums=1;
        }

        for(int i=0;i<thread_nums;++i){
            m_threads.emplace_back(std::thread([this]{
                while(is_running.load(std::memory_order_acquire)){
                    Task task;
                    {
                        // 获取锁
                        std::unique_lock<std::mutex> lock(m_mutex);
                        // 如果任务队列不为空或者线程池停止运行了就执行后面的代码
                        m_cond.wait(lock,[this]{return !is_running.load(std::memory_order_acquire)
                            || !m_tasks.empty();});
                        if(!is_running.load(std::memory_order_acquire)){
                            return;
                        }
                        task=std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            }));
        }
    }

    ~ThreadPool(){
        stop();
    }

    template <class Function,class ... Args>
    std::future<typename std::result_of<Function(Args...)>::type>   // 返回类型
    add(Function&& func,Args&&... args){
        // 声明返回值
        typedef typename std::result_of<Function(Args...)>::type return_type;
        auto it=std::make_shared<Task>(std::bind(std::forward<Function(func),std::forward<Args>(args)...));
        auto ret=it->get_future();
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(!is_running.load(std::memory_order_acquire)){
                throw std::runtime_error("thread pool has stopped");
            }
            m_tasks.emplace([it]{(*it)();})
        }
        m_cond.notify_one();
        return ret;
    }

    void stop() noexcept {
        // 修改原子变量并唤醒所有线程执行
        is_running.store(false,std::memory_order_release);
        m_cond.notify_all();
    }
private:
    // 取消左值和右值的拷贝构造函数和拷贝赋值运算符
    ThreadPool(ThreadPool&)=delete;
    ThreadPool& operator=(const ThreadPool&)=delete;
    ThreadPool(ThreadPool&&)=delete;
    ThreadPool& operator=(ThreadPool&&)=delete;
private:
    // 线程池线程数量
    size_t thread_nums=0;
    // 线程池是否正在工作中，默认没有运行
    std::atomic<bool> is_running={false};
    // 线程池中空闲线程的数量
    // size_t free_threads=0;
    // 线程池线程容器
    std::vector<std::thread> m_threads;
    // 任务队列
    std::queue<Task> m_tasks;
    // 任务队列的条件变量
    std::condition_variable m_cond;
    // 控制访问临界资源的互斥量
    std::mutex m_mutex;
};

}