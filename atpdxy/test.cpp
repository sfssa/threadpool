#include "thread.h"
#include <iostream>

static int shared=0;

void function(int i){
    std::cout<<"Task "<<i<<" begin working!"<<std::endl;
    ++shared;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"Task "<<i<<" finish work"<<std::endl;
}

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