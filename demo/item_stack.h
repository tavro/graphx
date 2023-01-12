#ifndef STACK_DEF
#define STACK_DEF

#include <string>

class ItemStack {
public:
    std::string item_type;
    int amount;

    ItemStack(std::string type) {
        item_type = type;
        amount = 1;
    }

    void add() {
        amount++;
    }

    void remove(int amt) {
        amount-=amt;
    }
};

#endif