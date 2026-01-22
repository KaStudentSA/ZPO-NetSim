//
// Created by adi on 15.01.2026.
//

#ifndef NETSIM_FACTORY_HXX
#define NETSIM_FACTORY_HXX
#include "types.hxx"

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
    void remove_by_id(ElementID id){remove_if(v.begin(), v.end(),
                        [id](Node& node) {
                          return node.id == id;
                        });}
    NodeCollection<Node>::iterator find_by_id(ElementID id) {return std::find_if(nodes_.begin(),nodes_.end(),
                    [id](Node& node) {
                        return node.id == id;
                    });}
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const {return std::find_if(nodes_.cbegin(),nodes_.cend(),
                    [id](Node& node) {
                        return node.id == id;
                    });}

private:
    container_t nodes_;

};
#endif //NETSIM_FACTORY_HXX