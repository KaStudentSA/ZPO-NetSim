#include "nodes.hxx"
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
