#include "package.hpp"

// Definicja statycznych pól klasy
std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

// Konstruktor domyślny
Package::Package() {
    if (!freed_IDs.empty()) {
        id = *freed_IDs.begin();
        freed_IDs.erase(freed_IDs.begin());
    } else {
        if (assigned_IDs.empty()) {
            id = 1;
        } else {
            id = *assigned_IDs.rbegin() + 1;
        }
    }
    assigned_IDs.insert(id);
}

// Konstruktor z konkretnym ID
Package::Package(ElementID id_input) : id(id_input) {
    assigned_IDs.insert(id);
    freed_IDs.erase(id);
}

// Konstruktor przenoszący
Package::Package(Package&& other) noexcept : id(other.id) {
    // ID zostaje przepisane, zbiory nie wymagają zmian,
    // bo liczba obiektów w systemie się nie zmienia.
}

// Operator przypisania przenoszący
Package& Package::operator=(Package&& other) noexcept {
    if (this != &other) {
        assigned_IDs.erase(this->id);
        freed_IDs.insert(this->id);
        this->id = other.id;
    }
    return *this;
}

// Destruktor
Package::~Package() {
    assigned_IDs.erase(id);
    freed_IDs.insert(id);
}