class CraftingUI {
public:
    Inventory* inventory;
    std::vector<Recipe*> recipes;
    int active_recipe = 0;

    ComponentUI component{0, 0, 224, 128, "CRAFTING", engine::DARK_GREY, engine::BLUE, engine::BLACK};

    bool is_open = false;

    CraftingUI(Inventory* inv) {
        ItemStack* ovens = new ItemStack("oven");
        RecipeItem needed{"rock", 8};

        ItemStack* chests = new ItemStack("chest");
        ItemStack* crafting_tables = new ItemStack("crafting-table");
        RecipeItem needed2{"wood", 8};

        ItemStack* bread = new ItemStack("bread");
        RecipeItem needed3{"wheat", 3};

        ItemStack* sword = new ItemStack("sword");
        ItemStack* shovel = new ItemStack("shovel");
        ItemStack* pick = new ItemStack("pickaxe");
        ItemStack* axe = new ItemStack("axe");
        ItemStack* hoe = new ItemStack("hoe");
        RecipeItem needed4{"wood", 2};
        RecipeItem needed5{"rock", 3};

        Recipe* oven_recipe = new Recipe(ovens, needed);
        Recipe* bread_recipe = new Recipe(bread, needed3);
        Recipe* chest_recipe = new Recipe(chests, needed2);
        Recipe* crafting_table_recipe = new Recipe(crafting_tables, needed2);

        Recipe* sword_recipe = new Recipe(sword, needed4, needed5);
        Recipe* shovel_recipe = new Recipe(shovel, needed4, needed5);
        Recipe* pick_recipe = new Recipe(pick, needed4, needed5);
        Recipe* axe_recipe = new Recipe(axe, needed4, needed5);
        Recipe* hoe_recipe = new Recipe(hoe, needed4, needed5);

        recipes.push_back(oven_recipe);
        recipes.push_back(chest_recipe);
        recipes.push_back(crafting_table_recipe);
        recipes.push_back(bread_recipe);
        recipes.push_back(sword_recipe);
        recipes.push_back(shovel_recipe);
        recipes.push_back(pick_recipe);
        recipes.push_back(axe_recipe);
        recipes.push_back(hoe_recipe);

        inventory = inv;
    }

    void add_recipe(Recipe* recipe) {
        recipes.push_back(recipe);
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
        if(recipes[active_recipe]->can_craft(*inventory)) {
            inventory->add_item(recipes[active_recipe]->craft(*inventory)->item_type);
            is_open = false;
        }
    }

    void previous_recipe() {
        if(--active_recipe < 0)
            active_recipe = recipes.size()-1;
    }

    void next_recipe() {
        if(++active_recipe > recipes.size()-1)
            active_recipe = 0;
    }

};