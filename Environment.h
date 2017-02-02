#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>

#include "Object.h"


class Environment {
public:
    Environment();

private:
    Environment* parent;
    std::unordered_map<std::string, ObjectFunction* > objectMap;
};

#endif /* ENVIRONMENT_H */

