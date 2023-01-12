class CraftingUI {
public:
    Inventory* inventory;
    std::vector<Recipe*> recipes;
    int active_recipe = 0;

    ComponentUI component{0, 0, 128, 128, "CRAFTING", engine::DARK_GREY, engine::BLUE, engine::BLACK};

    bool is_open = false;

    CraftingUI(Inventory* inv, Recipe* recipe) {
        recipes.push_back(recipe);
        inventory = inv;
    }

    std::vector<Line*> get_lines() {
        std::vector<Line*> lines;

        for(Recipe* recipe : recipes) {
            std::string final_str = recipe->result->item_type + ": ";
            for(RecipeItem item : recipe->needed_items) {
                final_str += item.item_type + " (";
                std::string amount = std::to_string(item.amount);
                final_str += amount + ") ";
            }
            if(recipe == recipes[active_recipe])
                lines.push_back(new Line{final_str, engine::WHITE});
            else
                lines.push_back(new Line{final_str, engine::DARK_GREY});
        }

        return lines;
    }

    void craft_active() {
        if(recipes[active_recipe]->can_craft(*inventory))
            inventory->add_item(recipes[active_recipe]->craft(*inventory)->item_type);
    }

    void next_recipe() {
        if(--active_recipe < 0)
            active_recipe = recipes.size()-1;
    }

    void previous_recipe() {
        if(++active_recipe > recipes.size()-1)
            active_recipe = 0;
    }

};