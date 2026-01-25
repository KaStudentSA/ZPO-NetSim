#ifndef PACKAGE_HXX
#define PACKAGE_HXX

#include <set>

#include "types.hpp"

/*
public:

Package() - konstruktor oraz id obsługa wyznaczania id

Package(ElementID) - ustawienie ręczne id

Package(Pacakge&&) - przepisanie wartości id z innego obiektu

operator=(Package&&) : Package& - to samo co wcześniej ale operatorem

get_id(): ElementID {query} - pobranie id

~Package - destruktor oraz zmniejsza kolejne id o 1

private:
ElementID id; - tworzenie int id

inline static std::set<ElementID> assigned_IDs; - przypisane id
inline static std::set<ElementID> freed_IDs; - uwolnione id
 */

class Package {
public:
    Package();
    Package(ElementID id_input);
    Package(Package&& other) noexcept;
    Package& operator=(Package&& other) noexcept;
    ~Package();

    ElementID get_id() const { return id; }

private:
    ElementID id;

    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif
