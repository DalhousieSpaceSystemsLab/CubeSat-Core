
#ifndef LORIS_MESSAGE_KEYVALUEPAIRCONTAINERINTERFACE_H_
#define LORIS_MESSAGE_KEYVALUEPAIRCONTAINERINTERFACE_H_

#include <vector>

//Interface for Key Value Pair Container
class KeyValuePairContainerInterface {
public:
    virtual void AddKeyValuePair(unsigned int, float) = 0;
    virtual void AddKeyValuePair(unsigned int, int) = 0;

    virtual std::vector<int> GetKeys() = 0;
    virtual float GetFloat(int index) = 0;
    virtual int GetInt(int index) = 0;
};

#endif
