class RecipeItem {
public:
    std::string item_type;
    int amount;

    RecipeItem(std::string type, int amt) {
        item_type = type;
        amount = amt;
    }
};