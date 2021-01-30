#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>


class GRAPH {
private:
    unsigned int _nbEdges, _nbNodes;
    std::string _fileName;
public:
    GRAPH();
    GRAPH(std::string const & file) : _nbEdges(0), _nbNodes(0), _fileName(file) {}
    virtual ~GRAPH() {}

    virtual void writeDotFile() = 0;
    virtual void colorGraph() = 0;

    std::string getFileName() const {
        return _fileName;
    }

    unsigned int getNbEdges() const {
        return _nbEdges;
    }

    unsigned int getNbNodes() const {
        return _nbNodes;
    }

    void setNbEdges(unsigned int const & edges) {
        _nbEdges = edges;
    }

    void setNbNodes(unsigned int const & nodes) {
        _nbNodes = nodes;
    }
};


class G_MATRIX: public GRAPH{
private:
    std::map<unsigned int, unsigned int> _sommetMap;
    std::vector<unsigned int> _matrix;
public:
    G_MATRIX();
    G_MATRIX(std::string const & file) : GRAPH(file), _matrix() {}

    void writeDotFile() override;
    void colorGraph() override;
    void createMatrix();

    std::vector<unsigned int> getMatrix()  {
        return _matrix;
    }

    void addMapElement(unsigned int index, unsigned int value) {
        _sommetMap.insert(std::pair<unsigned int, unsigned int>(index, value));
    }

    void addNumber(unsigned int number) {
        _matrix.push_back(number);
    }
};


class G_LIST: public GRAPH{
private:
    std::map<unsigned int, unsigned int> _sommetMap;
    std::map<unsigned int, std::vector<unsigned int>> _list;
public:
    G_LIST();
    G_LIST(std::string const & file) : GRAPH(file), _list() {}

    void writeDotFile() override;
    void colorGraph() override;
    void createList();

    std::map<unsigned int, std::vector<unsigned int>> getList() const {
        return _list;
    }

    void addMapElement(unsigned int index, unsigned int value) {
        _sommetMap.insert(std::pair<unsigned int, unsigned int>(index, value));
    }

    void addElement(unsigned int first, std::vector<unsigned int> second) {
        _list.insert(std::pair<unsigned int, std::vector<unsigned int>>(first, second));
    }
};

#endif // GRAPHE_H
