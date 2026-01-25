#include "storage_types.hpp"

Package PackageQueue::pop() {
    PackageQueueType t=queue_type;
    Package result;
    switch (t) {  // ostrzeżenie GCC: "Not all switch cases were handled"
        //   (bo brakuje przypadku `QueueType::Lifo`)
        case PackageQueueType::FIFO:
            result = std::move(packages_.front());
            packages_.pop_front();
            break;
        case PackageQueueType::LIFO:
            result = std::move(packages_.back());
            packages_.pop_back();
            break;
    }
    return result;
}

