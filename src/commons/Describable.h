#ifndef DESCRIBABLE_H
#define DESCRIBABLE_H

class Describable {
public:
    virtual const char *getDescription() const = 0;
};

#endif