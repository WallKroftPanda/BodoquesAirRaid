#include "menu.h"
#include <iostream>
using namespace std;
menu::menu(char* filename,GLuint shader_programme):malla(filename,shader_programme){
    cout<<"constructor de menu"<<endl;
}