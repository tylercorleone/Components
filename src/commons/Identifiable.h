#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

class Identifiable {
public:
    virtual const char *getIdentifier() const = 0;
};

#endif