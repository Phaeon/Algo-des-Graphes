#include <iostream>
#include <vector>
#include <math.h>

#include "graphe.h"

using namespace std;

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        std::cout << "Bad usage of the command." << std::endl;
        std::cout << "\tUsage: " << argv[0] << " <fileName>" << std::endl << std::endl;
        std::cout << "List of available .col files:" << std::endl;
        system("find . -name \"*.col\"");
        return EXIT_FAILURE;
    }

    std::cout << "___________-=WELCOME=-___________" << std::endl;
    std::cout << "Author: Jack Hogg" << std::endl;
    std::cout << "E-mail: jhogg@etud.univ-angers.fr" << std::endl;
    std::cout << "Year:   L3 Informatique 2020-21" << std::endl << std::endl;

    std::cout << "File used: " << argv[1] << std::endl << std::endl;

    std::cout << "Matrix in progress..." << std::endl;
    G_MATRIX g(argv[1]);
    try {
        g.createMatrix();
    } catch(int i) {
        return i;
    }
    std::cout << "Matrix made." << std::endl << std::endl;

    std::cout << "List in progress..." << std::endl;
    G_LIST g2(argv[1]);
    try {
        g2.createList();
    } catch(int i) {
        return i;
    }
    std::cout << "List made." << std::endl << std::endl;

    std::cout << "Making graph using matrix..." << std::endl;
    g.writeDotFile();
    std::cout << "Making graph using list..." << std::endl;
    g2.writeDotFile();

    std::cout << "____________!_ENJOY_!____________" << std::endl;

    return EXIT_SUCCESS;
}
