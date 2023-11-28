/*
 * @Author: atpdxy
 * @Date: 2023-11-28 09:41:52
 * @Version: 1.0
 * @LastEditors: sfssa 1664549131@qq.com
 * @LastEditTime: 2023-11-28 23:11:26
 * @Description: 
 */
#include "thread.h"
#include <iostream>
#include <cmath>
#include <thread>

// 测试共享访问数据的准确性
static int shared=0;

/**
 * @description: 模拟线程执行过程
 * @param {int} i 线程id
 * @return {*}
 */
void function(int i){
    std::cout<<"Task "<<i<<" begin working!"<<std::endl;
    if(i%2==0){
        ++shared;
    }else{
        --shared;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"Task "<<i<<" finish work"<<std::endl;
}

/**
 * @description: 检验拷贝构造函数和赋值运算符
 * @return {*}
 */
void test_for_copy(){
    atpdxy::ThreadPool pool(4);
    // 下面的代码都会导致编译报错
    // atpdxy::ThreadPool pool1=pool;
    // atpdxy::ThreadPool pool2(pool);
    // atpdxy::ThreadPool pool3(std::move(pool));
    // atpdxy::ThreadPool pool4=std::move(pool);
}

/**
 * @description: CPU密集型任务
 * @param {int} id
 * @return {*}
 */
void cpuIntensiveTask(int id){
    std::cout<<"CPU-bind task "<<id<<" start working"<<std::endl;
    double ret=0.0;
    for(int i=0;i<100000000;++i){
        ret+=std::sin(i)*std::cos(i);
    }
    std::cout<<"Thread "<<id<<" ret is: "<<ret<<std::endl;
    std::cout<<"CPU-bind task "<<id<<" finish work"<<std::endl;
}

/**
 * @description: IO密集型任务
 * @param {int} id
 * @return {*}
 */
void ioIntensiveTask(int id){
    std::cout<<"I/O-bind task "<<id<<" start working"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"I/O-bind task "<<id<<" finish work"<<std::endl;
}

void test(std::function<void(int)> task){
    int thread_nums=std::thread::hardware_concurrency();
    // int thread_nums=std::thread::hareare_concurrenty()+1;
    std::cout<<"This machine max thread numbers is: "<<thread_nums<<std::endl;
    atpdxy::ThreadPool pool(thread_nums);
    std::vector<std::future<void>> futures;
    for(int i=0;i<thread_nums;++i){
        futures.push_back(pool.addTask(task,i+1));
    }
    for(const auto& future:futures){
        future.wait();
    }
    pool.stop();
}

int main(){
    auto start=std::chrono::high_resolution_clock::now();
    // test_thread();
    // test_cpuIntensiveTask();
    test(cpuIntensiveTask);
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    // std::cout<<"The value of shared is "<<shared<<" Finally!"<<std::endl;
    std::cout<<"Totally exec for "<<duration.count()<<" microseconds"<<std::endl;
}