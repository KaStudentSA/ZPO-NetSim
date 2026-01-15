#include <list>
#include <package.hxx>



enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
    using const_iterator = std::list<Package>::const_iterator;
    void push(Package&& package);
    bool empty() const;
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    size_t size() const;
    virtual ~IPackageStockpile() =default;

};

class IPackageQueue {
    Package pop() ;
    Package get_queue_type() ;

};

class PackageQueue : public IPackageQueue {
    PackageQueue(PackageQueueType type) ;
};

