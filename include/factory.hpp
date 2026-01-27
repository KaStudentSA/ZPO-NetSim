//
// Created by adi on 15.01.2026.
//

#ifndef NETSIM_FACTORY_HXX
#define NETSIM_FACTORY_HXX
#include <list>

#include "nodes.hpp"
#include "types.hpp"
enum NodeColor {NIEODWIEDZONY, ODWIEDZONY, ZWERYFIKOWANY};

#include <istream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>

enum class ElementType {RAMP, WORKER, STOREHOUSE, LINK };
template <typename Node>
class NodeCollection {
public:

    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    iterator begin() { return nodes_.begin(); }
    iterator end() { return nodes_.end(); }
    const_iterator begin() const { return nodes_.begin(); }
    const_iterator end() const { return nodes_.end(); }
    const_iterator cbegin() const { return nodes_.cbegin(); }
    const_iterator cend() const { return nodes_.cend(); }

    void add(Node&& node){nodes_.emplace_back(std::move(node));}
    void remove_by_id(ElementID id) {
        nodes_.remove_if([id](const Node& node) {
            return node.get_id() == id;
        });
    }
    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(nodes_.begin(),nodes_.end(),
                    [id](Node& node) {
                        return node.get_id() == id;
                    });}
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {return std::find_if(nodes_.cbegin(),nodes_.cend(),
                    [id](const Node& node) {
                        return node.get_id() == id;
                    });}

private:
    container_t nodes_;

};

class Factory
{
public:
    // Ramps:
    void add_ramp(Ramp&& ramp) { ramps_.add(std::move(ramp)); }
    void remove_ramp(ElementID id) { ramps_.remove_by_id(id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramps_.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramps_.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramps_.cend(); }

    // Workers:
    void add_worker(Worker&& worker) { workers_.add(std::move(worker)); }
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return workers_.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return workers_.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return workers_.cend(); }

    //Storehouse:
    void add_storehouse(Storehouse&& storehouse) { storehouses_.add(std::move(storehouse)); };
    void remove_storehouse(ElementID id_input);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouses_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouses_.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouses_.cend(); }

    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

private:
    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id) {
        auto it = collection.find_by_id(id);
        if (it != collection.end()) {
            collection.remove_by_id(id);
        }
    }
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
};

struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};
ParsedLineData parse_line(std::string line);
void tokenize(std::string& line, std::vector<std::string>& ct, char delimiter);

Factory load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);

#endif //NETSIM_FACTORY_HXX