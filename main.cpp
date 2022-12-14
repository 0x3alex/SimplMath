#include <curses.h>
#include "./sclui/sclui.hpp" 
#include <random>
#define TO_SOLVE 5

using namespace sclui;

static std::vector<int> results = {};
static std::vector<BasicItem *> addresses = {};

TextBox *minimum = new TextBox("Minimum",1,3,4,COLOR_WHITE,COLOR_BLUE,&TextBoxFilterIsNumber, '|');;
TextBox *maximum = new TextBox("Maximum",1,5,4,COLOR_WHITE,COLOR_BLUE,&TextBoxFilterIsNumber, '|');
Screen game = Screen("Game",40,20,0,0);;

bool tbFilter(int c) {
    return isdigit(c) || c == '*' || c == '+' || c == '-';
}

//extra quit method to free pointers
void myQuit() {
    for(auto &n : addresses) {;
        delete n;
    }
    delete minimum;
    delete maximum;
    doQuit();
}

void doSolve() {
    for(int i = 0; i <= TO_SOLVE; i++) {
        TextBox *tb = (TextBox*) game.items.at(i);

        int r = -9999999;
        if(tb->value.size() > 0)
            r = std::stoi(tb->value);
        if(r == results.at(i)) {
            tb->setColor(COLOR_GREEN);
        }else{
            tb->setColor(COLOR_RED);
            tb->value = std::to_string(results.at(i));
        }
        tb->setInteractable(false);
        tb->setColorFocus(COLOR_WHITE);
    }
    game.getItemByName("Solve")->setVisible(false);
    game.update();
}

void gameScreen() {
    if(minimum->value.size() == 0 || maximum->value.size() == 0) return;
    int minVal = std::stoi(minimum->value);
    int maxVal = std::stoi(maximum->value);

    if(minVal >= maxVal) return;

    Button quit = Button("Quit",1,19,COLOR_WHITE,COLOR_RED);
    quit.onButtonPress = &myQuit;
    Button solve = Button("Solve",1,16,COLOR_WHITE,COLOR_GREEN);
    solve.onButtonPress = &doSolve;
    

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> nums(minVal, maxVal); // define the range
    std::uniform_int_distribution<> choices(1, 3); // define the range

    int num1,num2,choice,res,yOffet = 2;
    
    for(int i = 0; i <= TO_SOLVE; i++) {
        
        std::string textBoxText = "";
        num1 = nums(gen);
        num2 = nums(gen);
        choice = choices(gen);
        textBoxText.append(std::to_string(num1));
        switch(choice) {
            case 1:
                res = num1 + (num2);
                textBoxText.append("+");
                break;
            case 2:
                res = num1 - (num2);
                textBoxText.append("-");
                break;
            case 3:
                res = num1 * (num2);
                textBoxText.append("*");
                break;
        }
        textBoxText.append(std::to_string(num2));
        results.push_back(res);
        TextBox *tb = new TextBox(textBoxText,1,1+yOffet,7,COLOR_WHITE,COLOR_BLUE,&tbFilter, '=');
        addresses.push_back(tb);
        yOffet += 2;
        game.addItem(tb);
        game.centerItem(game.X,tb);
    }
    
    game.addItem(&solve);
    game.addItem(&quit);
    game.centerItem(game.X,&solve);
    game.centerItem(game.X,&quit);
    
    game.draw();
}


int main(void) {
    
    initSclui();
    Screen menu = Screen("Menu",40,14,0,0);

    Text text = Text("Welcome to SimplMath",1,1,COLOR_BLANK);
    
    Button play = Button("Play",1,1,COLOR_WHITE,COLOR_GREEN);
    play.onButtonPress = &gameScreen;
    Button quit = Button("Quit",1,13,COLOR_WHITE,COLOR_RED);
    quit.onButtonPress = &doQuit;


    menu.addItem(&text);
    menu.addItem(minimum);
    menu.addItem(maximum);
    menu.addItem(&play);
    menu.addItem(&quit);
    menu.centerItem(menu.X,&text);
    menu.centerItem(menu.X,minimum);
    menu.centerItem(menu.X,maximum);
    menu.centerItem(menu.XY,&play);
    menu.centerItem(menu.X,&quit);
    menu.draw();

    return 0;
}