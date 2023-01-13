#include "ui_component.h"
class InventoryUI {
public:
    Inventory* inventory;
    ComponentUI component{0, 0, 192, 128, "INVENTORY", engine::DARK_GREY, engine::BLUE, engine::BLACK};

    bool is_open = false;

    InventoryUI(Inventory* inv) {
        inventory = inv;
    }

    std::vector<Line*> get_lines() {
        std::vector<Line*> lines;

        for(ItemStack* inventory_item : inventory->item_stacks) {
            std::string amount = std::to_string(inventory_item->amount);
            if(inventory_item->item_type == inventory->get_active_item())
                lines.push_back(new Line(inventory_item->item_type + " - " + amount, engine::WHITE));
            else
                lines.push_back(new Line(inventory_item->item_type + " - " + amount, engine::DARK_GREY));
        }

        return lines;
    }

};