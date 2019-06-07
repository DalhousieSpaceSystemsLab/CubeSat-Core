
#ifndef LORIS_MESSAGE_KEYVALUEPAIRCONTAINER_H_
#define LORIS_MESSAGE_KEYVALUEPAIRCONTAINER_H_

#include "KeyIntPair.h"
#include "KeyFloatPair.h"
#include "KeyValuePairContainerInterface.h"
#include <vector>

class KeyValuePairContainer : public KeyValuePairContainerInterface {
public:
    KeyValuePairContainer();
    ~KeyValuePairContainer();
    void AddKeyValuePair(unsigned int key, float value) override ;
    void AddKeyValuePair(unsigned int key, int value) override;

    std::vector<int> GetKeys() override;
    float GetFloat(int index) override;
    int GetInt(int index) override;
    int GetAmountofFloatPairs();
private:
    std::vector<KeyIntPair> key_int_pairs_;
    std::vector<KeyFloatPair> key_float_pairs_;
};

#endif
