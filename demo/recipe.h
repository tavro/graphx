#include <vector>
#include "recipe_item.h"
#include "item_stack.h"

class Recipe {
public:
    ItemStack* result = nullptr;
    std::vector<RecipeItem> needed_items;

    Recipe(ItemStack* res, RecipeItem item) {
        result = res;
        needed_items.push_back(item);
    }

    bool can_craft(Inventory inventory) {
        for(RecipeItem item : needed_items) {
            if(inventory.get_item_amount(item.item_type) < item.amount)
                return false;
        }
        return true;
    }

    ItemStack* craft(Inventory inventory) {
        if(can_craft(inventory)) {
            for(RecipeItem item : needed_items) {
                inventory.remove_item(item.item_type, item.amount);
            }
            return result;
        }
        return nullptr;
    }
};