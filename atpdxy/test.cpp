/*
 * @Author: atpdxy
 * @Date: 2023-11-28 09:41:52
 * @Version: 1.0
 * @LastEditors: sfssa 1664549131@qq.com
 * @LastEditTime: 2023-11-28 15:56:31
 * @Description: 
 */
#include "thread.h"
#include <iostream>

static int shared=0;

/**
 * @description: 模拟线程执行过程
 * @param {int} i 线程id
 * @return {*}
 */
void function(int i){
    std::cout<<"Task "<<i<<" begin working!"<<std::endl;
    ++shared;
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
 * @description: 检验线程池的执行
 * @return {*}
 */
void test_thread(){
    atpdxy::ThreadPool pool(4);
    std::vector<std::future<void>> futures;
    for(int i=0;i<5;++i){
        futures.push_back(pool.addTask(function,i+1));
    }

    for(auto& future:futures){
        future.wait();
    }
    pool.stop();
}

int main(){
    auto start=std::chrono::high_resolution_clock::now();
    test_thread();
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout<<"The value of shared is "<<shared<<" Finally!"<<std::endl;
    std::cout<<"Totally exec for "<<duration.count()<<" microseconds"<<std::endl;
}