#ifndef __TYPE_HH__
#define __TYPE_HH__

#include <variant>
#include "ast/defs.hh"

class TypeId : public Type {
    sptr<Identifier> _id;
public:
    TypeId(const loc_type& loc, sptr<Identifier> id)
        : Type(loc), _id(id) {}
    
    void print(std::ostream& os, int level) const override;
};

class ArrayType : public Type {
    sptr<Type> _baseType;
public:
    ArrayType(const loc_type& loc, sptr<Type> baseType)
        : Type(loc), _baseType(baseType) {}

    void print(std::ostream& os, int level) const override;
};

class RecordType : public Type {
public:
    using RecordComponent = std::pair<sptr<Identifier>, sptr<Type>>;
    RecordType(const loc_type& loc, std::vector<RecordComponent>& components)
        : Type(loc), _components(std::move(components)) {}

    void print(std::ostream& os, int level) const override;
private:
    std::vector<RecordComponent> _components;
};

#endif // ! __TYPE_HH__
