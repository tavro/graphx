#include <vector>
#include "item_stack.h"

class Inventory {
public:
    std::vector<ItemStack*> item_stacks;

    Inventory() {

    }
    
    void add_item_stack(ItemStack* stack) {
        item_stacks.push_back(stack);
    }

    void add_item(std::string type) {
        for(ItemStack* stack : item_stacks) {
            if(stack->item_type == type) {
                stack->add();
                return;
            }
        }
        add_item_stack(new ItemStack(type));
    }

    void remove_item(std::string type, int amount) {
        for(ItemStack* stack : item_stacks) {
            if(stack->item_type == type) {
                stack->remove(amount);
                return;
            }
        }
    }

    int get_item_amount(std::string type) {
        for(ItemStack* stack : item_stacks) {
            if(stack->item_type == type) {
                return stack->amount;
            }
        }
        return 0;
    }
};