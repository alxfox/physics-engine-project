#ifndef GP_COMMON_UTILS_TYPEID_H
#define GP_COMMON_UTILS_TYPEID_H

namespace gp
{

/**
 * Unique typeid for classes.
 *
 * This class provides a unique id for a class, implemented by a function pointer.
 * Use {@link type_id()} to the the id.
 *
 * Taken from: http://codereview.stackexchange.com/questions/48594/unique-type-id-no-rtti
 */
class typeid_t
{
    using sig = typeid_t();

    sig* id;
    typeid_t(sig* id) : id{id} {}

public:
    template<typename T>
    friend typeid_t type_id();

    bool operator==(typeid_t o) const { return id == o.id; }
    bool operator!=(typeid_t o) const { return id != o.id; }
};

/**
 * @returns A unique if for the class
 */
template<typename T>
typeid_t type_id() { return &type_id<T>; }

}

#endif // GP_COMMON_UTILS_TYPEID_H