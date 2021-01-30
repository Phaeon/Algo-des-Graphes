#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <math.h>

#include "graphe.h"
#include "colors.h"


void G_MATRIX::createMatrix() {
    std::ifstream file(getFileName().c_str());

    if (file) {
        std::string ligne;
        while(getline(file, ligne)) {
            std::smatch matches;

            if (std::regex_search(ligne, matches, std::regex {"^p edge (\\d+) (\\d+)$"})) {
                setNbNodes(static_cast<unsigned int>(std::atoi(matches[1].str().c_str())));
                setNbEdges(static_cast<unsigned int>(std::atoi(matches[2].str().c_str())));

                for (unsigned int i = 0; i < getNbNodes() * getNbNodes(); i++) {
                    addNumber(0);
                }

            } else if (std::regex_search(ligne, matches, std::regex {"^e (\\d+) (\\d+)$"})) {
                unsigned int match1 = static_cast<unsigned int>(std::atoi(matches[1].str().c_str()));
                unsigned int match2 = static_cast<unsigned int>(std::atoi(matches[2].str().c_str()));

                _matrix.at((match2 - 1) + getNbNodes() * (match1 - 1)) = 1;
                _matrix.at((match1 - 1) + getNbNodes() * (match2 - 1)) = 1;
            }
        }
    } else {
        std::cout << "Le fichier " << getFileName() << " n'a pu ouvrir." << std::endl;
        throw EXIT_FAILURE;
    }
}


void G_LIST::createList() {
    std::ifstream file(getFileName().c_str());

    if (file) {
        std::string ligne;
        while(getline(file, ligne)) {
            std::smatch matches;

            if (std::regex_search(ligne, matches, std::regex {"^p edge (\\d+) (\\d+)$"})) {
                setNbNodes(static_cast<unsigned int>(std::atoi(matches[1].str().c_str())));
                setNbEdges(static_cast<unsigned int>(std::atoi(matches[2].str().c_str())));


                for (unsigned int i = 0; i < getNbNodes(); i++) {
                    std::vector<unsigned int> ligneV;
                    addElement(i+1, ligneV);
                }

            } else if (std::regex_search(ligne, matches, std::regex {"^e (\\d+) (\\d+)$"})) {
                unsigned int match1 = static_cast<unsigned int>(std::atoi(matches[1].str().c_str()));
                unsigned int match2 = static_cast<unsigned int>(std::atoi(matches[2].str().c_str()));

               _list.at(match1).push_back(match2);
               _list.at(match2).push_back(match1);
            }
        }

        //setList();
    } else {
        std::cout << "Le fichier " << getFileName() << " n'a pu ouvrir." << std::endl;
        throw EXIT_FAILURE;
    }
}


void G_MATRIX::writeDotFile() {
    std::string fileName("graphByMatrix.dot");
    std::ofstream file(fileName);

    file << "digraph {\n";
    file << "\tgraph [concentrate=true,label=\"" << fileName << "\", " << "labelloc=top, labeljust=left, fontsize=14];\n";

    colorGraph();
    shuffleColors();

    //Nodes
    for (auto s : _sommetMap) {
        std::string couleur = getColor(_sommetMap.at(s.first));

        file << "\t" << s.first << "[";
        file << "shape=circle, style=filled, color=" << couleur << ",label=\"" << s.first << "(" << _sommetMap.at(s.first) << ")\"";
        file << "]\n";

    }

    //Edges
    for (unsigned int i(0); i < getNbNodes(); ++i) {
        for (unsigned int j(0); j < getNbNodes(); ++j) {
            if (getMatrix().at(j+getNbNodes()*i) == 1) {
                file << "\t" << i+1 << " -> " << j+1 << "[arrowhead=none]\n";
            }
        }
    }

    file << "}";
    file.close();

    std::string dotcmd = "dot -Tpng " + fileName + " -o " + fileName + ".png";
    std::cout << "Calling : " << dotcmd << std::endl;
    if (std::system((dotcmd).c_str()) != 0) {
        std::cerr << "Command failed." << std::endl;
        return;
    }
    std::cout << "Created graphByMatrix.dot.png" << std::endl;
    std::cout << std::endl;
}


void G_LIST::writeDotFile() {
    std::string fileName("graphByList.dot");
    std::ofstream file(fileName);

    colorGraph();
    shuffleColors();

    file << "digraph {\n";
    file << "\tgraph [concentrate=true,label=\"" << fileName << "\", " << "labelloc=top, labeljust=left, fontsize=14];\n";


    //Nodes
    for (auto s : _sommetMap) {
        std::string couleur = getColor(_sommetMap.at(s.first));

        file << "\t" << s.first << "[";
        file << "shape=circle, style=filled, color=" << couleur << ",label=\"" << s.first << "(" << _sommetMap.at(s.first) << ")\"";
        file << "]\n";

    }

    //Edges
    for (auto parent : getList()) {
        for (auto neighbour : parent.second) {
            file << "\t" << parent.first << " -> ";
            file << neighbour << "[arrowhead=none]";
            file << "\n";
        }
    }

    file << "}";
    file.close();

    std::string dotcmd = "dot -Tpng " + fileName + " -o " + fileName + ".png";
    std::cout << "Calling : " << dotcmd << std::endl;
    if (std::system((dotcmd).c_str()) != 0) {
        std::cerr << "Command failed." << std::endl;
        return;
    }
    std::cout << "Created graphByList.dot.png" << std::endl;
    std::cout << std::endl;
}

void G_LIST::colorGraph() {
    // Initialisation des couleurs
    for (unsigned int i(1); i <= getNbNodes(); i++) {
        addMapElement(i, 0);
    }

    _sommetMap.at(1) = 1;

    // For every element, we check if the adjacent nodes have the same color (count),
    // if that's the case, "count" gets incremented until a new color is found.
    for (auto parent : getList()) {
        if (_sommetMap.at(parent.first) != 1) {
            unsigned int count = 1;
            for (auto s : parent.second) {
                if (_sommetMap.at(s) == count)
                    count++;
            }
            _sommetMap.at(parent.first) = count;
        }
    }
}

void G_MATRIX::colorGraph() {
    // Initialisation des couleurs
    for (unsigned int i(1); i <= getNbNodes(); i++) {
        addMapElement(i, 0);
    }

    _sommetMap.at(1) = 1;

    // For every element, we check if the adjacent nodes have the same color (count),
    // if that's the case, "count" gets incremented until a new color is found.
    for (unsigned int i(0); i < getNbNodes(); ++i) {
        if (_sommetMap.at(i+1) != 1) {
            unsigned int count = 1;
            for (unsigned int j(0); j < getNbNodes(); ++j) {
                if (getMatrix().at(j+getNbNodes()*i) == 1) {
                    if (_sommetMap.at(j+1) == count)
                        count++;
                }
            }
            _sommetMap.at(i+1) = count;
        }
    }
}




