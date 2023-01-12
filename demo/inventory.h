#include <vector>
#include "item_stack.h"

class Inventory {
public:
    std::vector<ItemStack*> item_stacks;
    int active_item = 0;

    Inventory() {

    }
    
    std::string get_active_item() {
        return item_stacks[active_item]->item_type;
    }

    void previous_item() {
        if(--active_item < 0)
            active_item = item_stacks.size()-1;
    }

    void next_item() {
        if(++active_item > item_stacks.size()-1)
            active_item = 0;
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