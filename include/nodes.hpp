//
// Created by adi on 15.01.2026.
//

#ifndef NETSIM_NODES_HXX
#define NETSIM_NODES_HXX
#include <memory>
#include <map>
#include <list>
#include <optional>

#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"
#include "helpers.hpp"
enum ReceiverType {WORKER, STOREHOUSE};
using const_iterator = std::list<Package>::const_iterator;

class IPackageReceiver {
public:
    virtual void receive_package(Package&&) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual const_iterator end() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = default_probability_generator): pg_(std::move(pg)) {};

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver() const;
    const preferences_t& get_preferences() const {return preferences_;};

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
};
class Storehouse: public IPackageReceiver, public IPackageStockpile{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO));

    void receive_package(Package&& p) override;
    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return receiverType_;};

    void push(Package&& package) override {d_->push(std::move(package));};
    bool empty() const override {return d_->empty();};
    const_iterator begin() const override {return d_->begin();};
    const_iterator end() const override {return d_->end();};
    const_iterator cbegin() const override {return d_->cbegin();};
    const_iterator cend() const override {return d_->cend();};
    size_t size() const override {return d_->size();};

private:
    ElementID id_;
    std::unique_ptr<PackageQueue> d_;
    ReceiverType receiverType_ = STOREHOUSE;
};

class PackageSender
{
public:
    ReceiverPreferences receiver_preferences;
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    void send_package();
    void push_package(Package&& p);
    const std::optional<Package>& get_sending_buffer() const
    {
        return sending_buffer_;
    };
private:
    std::optional<Package> sending_buffer_;
};

class Ramp : public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di)
        : id_(id), delivery_interval_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const
    {
        return delivery_interval_;
    };
    ElementID get_id() const
    {
        return id_;
    };
private:
    TimeOffset delivery_interval_;
    ElementID id_;
};

class Worker: public PackageSender
    {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue>);
    void do_work(Time t);
    TimeOffset get_processing_duration() const
    {
        return processing_duration_;
    };
    std::optional<Time> get_package_processing_start_time() const
    {
        return processing_start_time_;
    };
private:
    ElementID id_;
    TimeOffset processing_duration_;
    std::optional<Time> processing_start_time_;
    std::unique_ptr<IPackageQueue> queue_;
    std::optional<Package> sending_buffer_;
    };

#endif //NETSIM_NODES_HXX