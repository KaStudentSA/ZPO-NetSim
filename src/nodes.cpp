#include "nodes.hpp"

void Storehouse::receive_package(Package&& p) {
    d_ -> push(std::move(p));
}
void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    double size = preferences.size();
    double probability = size / (size + 1);

    if (preferences.empty()) preferences.emplace(r, 1.);
    else {
        for (auto& elem: preferences) {
            elem.second *= probability;
        }
        preferences.emplace(r, 1 / (size + 1));
    }
}

void ReceiverPreferences::remove_receiver (IPackageReceiver* r) {
    preferences.erase(r);
    double size = preferences.size();
    double pb = (size + 1) / size;
    for (auto& elem: preferences) {
        elem.second *= pb;
    }
}
IPackageReceiver *ReceiverPreferences::choose_receiver() const {
    const double dist = pg_();
    double sum = 0;
    for (const auto& elem: preferences) {
        sum += elem.second;
        if (sum >= dist) {
            return elem.first;
        }
    }
    return nullptr;
}
void PackageSender::send_package() {
    if (sending_buffer) receiver_preferences_.choose_receiver()->receive_package(std::move(sending_buffer.value()));
    sending_buffer = std::nullopt;
};
void Ramp::deliver_goods(Time t) {
    if (delivery_interval_ == 1) {
        Package package = Package();
        push_package(std::move(package));
    }
    if (t % delivery_interval_ == 1) {
        Package package = Package();
        push_package(std::move(package));
    }
};
void Worker::do_work(Time t) {
    if (pd_ == 1) {
        if (!queue_->empty()) {
            push_package(queue_->pop());
            start_time_ = t;
        }
    } else {
        if (!sending_buffer.has_value()) {
            if (!queue_->empty()) {
                sending_buffer.emplace(queue_->pop());
                start_time_ = t;
            }
        }
        if ((t - start_time_) == pd_ - 1) {
            push_package(std::move(sending_buffer.value()));
            sending_buffer = std::nullopt;
        }
    }

};
