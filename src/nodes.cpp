#include "nodes.hpp"
void Storehouse::receive_package(Package&& p) {
    d_ -> push(std::move(p));
}
void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    preferences_.emplace(r, 1);
    for (auto &[fst, snd] : preferences_)
    {
        snd = 1 / static_cast<double>(preferences_.size());
    }
}

void ReceiverPreferences::remove_receiver (IPackageReceiver* r) {
    preferences_.erase(r);
    if (!preferences_.empty()) {
        for (auto & preference : preferences_) {
            preference.second = 1 / static_cast<double>(preferences_.size());
        }
    }
}
IPackageReceiver *ReceiverPreferences::choose_receiver() const {
    const double dist = probability_generator();
    double sum_of_ps = 0;
    for (const auto &[fst, snd] : preferences_) {
        sum_of_ps += snd;
        if (sum_of_ps > dist) {
            return fst;
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
