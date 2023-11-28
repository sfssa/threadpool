/*
 * @Author: atpdxy
 * @Date: 2023-11-28 15:19:41
 * @Version: 1.0
 * @LastEditors: sfssa 1664549131@qq.com
 * @LastEditTime: 2023-11-28 15:43:21
 * @Description: 作为基类设置派生类无法复制的属性
 */
#pragma once

namespace atpdxy{

class Noncopyable{
public:
    Noncopyable()=default;
    ~Noncopyable()=default;
    Noncopyable& operator=(const Noncopyable&)=delete;
    Noncopyable(const Noncopyable&)=delete;
    Noncopyable& operator=(Noncopyable&&)=delete;
    Noncopyable(Noncopyable&&)=delete;
};

}