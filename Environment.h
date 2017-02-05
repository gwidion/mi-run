#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>

#include "Object.h"

class Environment {
public:
    Environment();
    Environment( Environment& environment );

    Object* getObject(std::string key);
    void addObject( std::string key, Object* object );

    void mark();
    void markWithParents();
private:
    Environment* parent;
    std::unordered_map<std::string, Object*> objectMap;
};

#endif /* ENVIRONMENT_H */

