#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>

#include "Object.h"

class Environment {
public:
    Environment();

    Object* getObject(std::string key) {
        return objectMap[key];
    }

    void mark();
    void markWithParents();
private:
    Environment* parent;
    std::unordered_map<std::string, Object*> objectMap;
};

#endif /* ENVIRONMENT_H */

