//
// Created by adi on 15.01.2026.
//

#ifndef NETSIM_NODES_HXX
#define NETSIM_NODES_HXX
#include <memory>
#include <map>
#include <list>
#include "package.hxx"
#include "storage_types.hxx"
#include "types.hxx"
#include "helpers.hxx"
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
class Storehouse: public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::LIFO));
    void receive_package(Package&& p) override;
    ElementID get_id() const override {return id_;};
    ReceiverType get_receiver_type() const override {return STOREHOUSE;};

    const_iterator cbegin() const override {return d_->cbegin();}
    const_iterator begin() const override {return d_->begin();}
    const_iterator cend() const override { return d_->cend();}
    const_iterator end() const override { return d_->end();}
    IPackageStockpile* get_queue() const {return d_.get();}

    IPackageStockpile* get_id() {return d_.get();};
private:
    ElementID id_;
    std::unique_ptr<PackageQueue> d_;
};

#endif //NETSIM_NODES_HXX