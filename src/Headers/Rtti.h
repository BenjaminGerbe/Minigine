#include <string>
template<class Base>
struct RTTI
{
    std::string type;
    RTTI(const char* tname):type(tname){}

    bool IsExactlyA(const RTTI<Base>& other) const{
        return this->type == other.type;
    }
};
