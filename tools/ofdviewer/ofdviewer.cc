#include <stdlib.h>
#include <string>
#include <iostream>

//extern int imgui_main(int, char**);
extern int sdl_main(int, char**);
 
int main(int argc, char *argv[]){
    char *env_gui = getenv("GUI");
    std::string gui;
    if ( env_gui != nullptr ){
        gui = std::string(env_gui);
    } else {
        gui = "sdl";
    }
    if ( gui == "imgui" ){
        //return imgui_main(argc, argv);
    } else if ( gui == "sdl" ){
        return sdl_main(argc, argv);
    } else {
        std::cout << "Set enviornment variable GUI to 'imgui' or 'sdl'." << std::endl;
        return -1;
    }
}

