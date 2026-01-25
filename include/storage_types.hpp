#ifndef STORAGE_TYPES_HXX
#define STORAGE_TYPES_HXX

#include <list>
#include <package.hpp>

using const_iterator = std::list<Package>::const_iterator;
enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:

    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const= 0;
    virtual const_iterator cend() const = 0;
    virtual size_t size() const = 0;
    virtual ~IPackageStockpile() =default;

};

class IPackageQueue : public IPackageStockpile {
    public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const =0;

};

class PackageQueue : public IPackageQueue {
    public:
    PackageQueue(PackageQueueType type) {queue_type = type; }
    void push(Package&& package){packages_.emplace_back(std::move(package));}
    const_iterator begin() const override { return packages_.begin(); }
    const_iterator end() const override { return packages_.end(); }
    const_iterator cbegin() const override { return packages_.cbegin(); }
    const_iterator cend() const override { return packages_.cend(); }
    size_t size() const override { return packages_.size(); }
    bool empty() const override { return packages_.empty(); }
    PackageQueueType get_queue_type() const override { return queue_type; }
    Package pop() override;

    private:
    PackageQueueType queue_type;
    std::list<Package> packages_;
};

#endif
