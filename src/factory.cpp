//
// Created by Anastazja on 22.01.2026.
//
#include "factory.hpp"

#include <stdexcept>

/*
is_consistent() – sprawdzanie spójności sieci

do_deliveries() – dokonywanie ewentualnych dostaw na rampy

do_package_passing() – dokonywanie ewentualnego
przekazywania półproduktów

do_work() – dokonywanie ewentualnego przetwarzania
półproduktów przez robotników

(W praktyce każda z metod do_XXX() powinna jedynie wywoływać
właściwe metody z klas Ramp i Worker dla każdego elementu z
właściwej kolekcji węzłów.)
*/


bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if ( node_colors[sender]== ZWERYFIKOWANY) {
        return true;
    }
    node_colors[sender] = ODWIEDZONY;

    if (sender->receiver_preferences_.preferences.empty()) {
        throw std::logic_error("The sender has no recipients!");
    }

    bool has_another_reciever = false;
    for (const auto& receiver : sender->receiver_preferences_.preferences) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            has_another_reciever = true;
        }else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            IPackageReceiver* receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) {
                continue;
            }

            has_another_reciever = true;

            if (node_colors[sender] == NIEODWIEDZONY) {
                has_reachable_storehouse(sender, node_colors);
            }
        }
    }
    node_colors[sender] = ZWERYFIKOWANY;

    if (has_another_reciever) {
        return true;
    }else {
        throw std::logic_error("The sender has no recipients!");
    }
}


bool Factory::is_consistent()
{
  std::map<const PackageSender*, NodeColor> net_color;

  for (const auto& ramp : ramps_) {
    net_color[&ramp] = NIEODWIEDZONY;
  }
  for (const auto& worker : workers_) {
    net_color[&worker] = NIEODWIEDZONY;
  }


  try {
    for (const auto& ramp : ramps_) {
      has_reachable_storehouse(&ramp, net_color);
    }
  } catch (std::logic_error&) {
    return false;
  }
  return true;
}

void Factory::do_deliveries(Time t)
{

}

void Factory::do_package_passing()
{

}

void Factory::do_work(Time t)
{

}

void tokenize(std::string& line, std::vector<std::string>& ct, char delimiter) {
  std::istringstream stream(line);
  std::string token;
  while (getline(stream, token, delimiter)) {
    ct.push_back(token);
  }
};

ParsedLineData parse_line(std::string line) {
  ParsedLineData parsed_line;


  int marker = 0;
  std::vector<std::string> token_vec;
  char delimiter = ' ';
  std::stringstream ss(line);
  std::string token;
  while (getline(ss, token, delimiter)) {
    if (marker == 0) {
      if (token == "LOADING_RAMP") {
        parsed_line.element_type = ElementType::RAMP;
        marker += 1;
        continue;
      }
      if (token == "WORKER") {
        parsed_line.element_type = ElementType::WORKER;
        marker += 1;
        continue;
      }
      if (token == "STOREHOUSE") {
        parsed_line.element_type = ElementType::STOREHOUSE;
        marker += 1;
        continue;
      }
      if (token == "LINK") {
        parsed_line.element_type = ElementType::LINK;
        marker += 1;
        continue;
      }
    }
    token_vec.push_back(token);
  }
  for (auto& elem : token_vec) {
    delimiter = '=';
    std::vector<std::string> pair;
    tokenize(elem, pair, delimiter);
    parsed_line.parameters[pair[0]] = pair[1];
  }
  return parsed_line;
};

void save_factory_structure(Factory& factory, std::ostream& os) {
  // Rams
  std::vector<int> id_ramps;
  for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); it++) {
    id_ramps.push_back(it->get_id());
  }
  std::sort(id_ramps.begin(), id_ramps.end());

  os << "; == LOADING RAMPS ==" << std::endl << std::endl;
  for (const auto ID : id_ramps) {
    auto iter = factory.find_ramp_by_id(ID);
    os << "LOADING_RAMP id=" << iter->get_id() << " delivery-interval=" << iter->get_delivery_interval()
    << std::endl;
    os << std::endl;
  }
  // Workers
  std::vector<int> id_workers;
  for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); it++) {
    id_workers.push_back(it->get_id());
  }
  std::sort(id_workers.begin(), id_workers.end());
  os << "; == WORKERS ==" << std::endl << std::endl;
  for (const auto ID : id_workers) {
    auto iter = factory.find_worker_by_id(ID);
    os << "WORKER id=" << iter->get_id() << " processing-time=" << iter -> get_processing_duration()
    << " queue-type=" << get_queue_type() << std::endl;
    os << std::endl;
  }
  // Storehouse
  std::vector<int> id_storehouses;
  for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); it++) {
    id_storehouses.push_back(it->get_id());
  }
  os << "; == STOREHOUSES ==" << std::endl << std::endl;
  for (const auto ID : id_storehouses) {
    auto iter = factory.find_storehouse_by_id(ID);
    os << "STOREHOUSE id=" << iter->get_id() << std::endl;
  }
  // Links
  os << "; == LINKS ==" << std::endl << std::endl;
  // teraz zapisuje się po koleje RAMP -> WORKER -> STOREHOUSE
  for (const auto ID : id_ramps) {
    auto iter = factory.find_ramp_by_id(ID);
    std::vector<int> id_worker;
    std::vector<int> id_storehouse;
    for (const auto receiver: iter -> receiver_preferences.get_preferences() ) {
      switch (receiver.first->get_receiver_type()) {
        case ReceiverType::WORKER:
          id_worker.push_back(receiver.first->get_id());
        continue;
        case ReceiverType::STOREHOUSE:
          id_storehouse.push_back(receiver.first->get_id());
        continue;
      }
    }
    std::sort(id_worker.begin(), id_worker.end());
    std::sort(id_storehouse.begin(), id_storehouse.end());

    for (auto id: id_storehouse) os << "LINK src=ramp-" << ID << " dest=store-" << id << std::endl;
    for (auto id: id_worker) os << "LINK src=ramp-" << ID << " dest=worker-" << id << std::endl;
    os << std::endl;
  }
};
Factory load_factory_structure(std::istream& is) {
  ParsedLineData parsed_line;
  Factory factory;
  std::string l;
  std::string id = "id";
  while (std::getline(is, l)) {
    if (l.empty() or l.front() == ';') {
      continue;
    }
    parsed_line = parse_line(l);
    if (parsed_line.element_type == ElementType::RAMP) {
      Ramp ramp(std::stoi(parsed_line.parameters[id]), std::stoi(parsed_line.parameters["delivery-interval"]));
      factory.add_ramp(std::move(ramp));
    }
    if (parsed_line.element_type == ElementType::WORKER) {
      Worker worker(std::stoi(parsed_line.parameters[id]), std::stoi(parsed_line.parameters["processing-time"]), std::stoi(parsed_line.parameters["queue-type"]));
      factory.add_worker(std::move(worker));
    }
    if (parsed_line.element_type == ElementType::STOREHOUSE) {
      Storehouse storehouse(std::stoi(parsed_line.parameters[id]));
      factory.add_storehouse(std::move(storehouse));
    }
    if (parsed_line.element_type == ElementType::LINK) {
      std::vector<std::string> src;
      std::vector<std::string> dest;
      char delimeter = '-';

      tokenize(parsed_line.parameters["src"], src, delimeter);
      tokenize(parsed_line.parameters["dest"], dest, delimeter);
      if (src[0] == "ramp") {
        if (dest[0] == "worker") {
          auto const ramp_iter = factory.find_ramp_by_id(std::stoi(src[1]));
          auto const worker_iter = factory.find_worker_by_id(std::stoi(dest[1]));
          ramp_iter->receiver_preferences_.add_receiver(&*worker_iter);
        }
        if (dest[0] == "store") {
          auto const ramp_iter = factory.find_ramp_by_id(std::stoi(src[1]));
          auto const store_iter = factory.find_storehouse_by_id(std::stoi(src[1]));
          ramp_iter->receiver_preferences_.add_receiver(&*store_iter);
        }
      }
      if (src[0] == "worker") {
        if (dest[0] == "worker") {
          auto const worker1_iter = factory.find_worker_by_id(std::stoi(src[1]));
          auto const worker2_iter = factory.find_worker_by_id(std::stoi(dest[1]));
          worker1_iter->receiver_preferences_.add_receiver(&*worker2_iter);
        }
        if (dest[0] == "store") {
          auto const worker_iter = factory.find_worker_by_id(std::stoi(src[1]));
          auto const store_iter = factory.find_storehouse_by_id(std::stoi(src[1]));
          worker_iter->receiver_preferences_.add_receiver(&*store_iter);
        }
      }
    }
  }
  return factory;
};
