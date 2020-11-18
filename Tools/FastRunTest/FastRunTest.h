//
// FastRunTest.h
// Note: When every RunTest finished, it should be pushed into Cakes(Backups).

#pragma once

#define TestName "NULL"

/*--------------------------------------------------------------------
                        fast code test boundary
---------------------------------------------------------------------*/

#include <string>
#include <vector>
#include <mutex>
#include <memory>

#define _name_of(x)   #x

class IResHandler
{
public:

    virtual ~IResHandler() { }

protected:

    void Free() { delete this; }

    void Add(std::shared_ptr<IResHandler> son) { child.push_back(son); }

    std::string GetName() { return name; }

private:

    friend class ResourcePool;

    std::string name;

    std::vector<std::shared_ptr<IResHandler>> child;
};

class ResourcePool
{
private:

    ResourcePool() = default;
    static ResourcePool* resp;

    static std::mutex read;
    static std::mutex write;

    struct Resource
    {
        std::vector<std::shared_ptr<IResHandler>> ptrs;
    }refs;

public:

    static ResourcePool*& Get()
    {
        std::unique_lock<std::mutex> lock_w(write);

        if (resp == nullptr)
            resp = new ResourcePool;

        return resp;
    }

    void Push(std::shared_ptr<IResHandler> ptr, const std::string& name)
    {
        ptr->name = name;

        std::unique_lock<std::mutex> lock_r(read);
        std::unique_lock<std::mutex> lock_w(write);

        refs.ptrs.push_back(ptr);
    }

    void Free()
    {
        std::unique_lock<std::mutex> lock_r(read);
        std::unique_lock<std::mutex> lock_w(write);

        delete this;

        // Exit...
    }
};

ResourcePool* ResourcePool::resp = nullptr;
std::mutex ResourcePool::read;
std::mutex ResourcePool::write;

struct SomeData : public IResHandler
{
    float data1;
    int data2;

    ~SomeData() { std::cout << "Kill SomeData...\n"; }
};

class Test : public IResHandler
{
private:

    Test() = default;
    void End() { Free(); std::cout << "After Exit Test!\n"; }

    //SomeData* data;

public:

    friend class Test;

    static Test* Get()
    {
        static int id = 0;
        std::shared_ptr<Test> temp(new Test);
        std::shared_ptr<SomeData> data = std::make_shared<SomeData>();
        temp->Add(data);

        ResourcePool::Get()->Push(temp, _name_of(Test) + std::to_string(id++));
        return temp.get();
    }
    static void SafeEnd(Test*& ptr) { ptr->End(); ptr = nullptr; }

    ~Test() { std::cout << "Exit Test name " << GetName() << "!\n"; }
};

class A : public IResHandler
{
public:
    A() {}
    A(int a) {}
    virtual ~A() {}
};

class B
{
public:
    int x = 0;
    A a;
    B() { std::cout << "default B\n"; }
    B(const B& b) { std::cout << "Copy B\n"; }
    B& operator=(const B& b) { std::cout << "Equal B\n"; return *this; }

    virtual ~B() {}
};

#include <thread>

void access(int id)
{
    std::string str = "thread # " + std::to_string(id) + "\n";

    std::cout << str;
}

/*--------------------------------------------------------------------
                        fast code test boundary
---------------------------------------------------------------------*/