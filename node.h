//
// Created by juan-diego on 3/11/24.
//

#ifndef HOMEWORK_GRAPH_NODE_H
#define HOMEWORK_GRAPH_NODE_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

// Color por defecto de un vertice (usado por SFML)
sf::Color default_node_color = sf::Color(150, 40, 50);
// Radio por defecto de un vertice (usado por SFML)
float default_radius = 0.4f;


struct Edge;


// *
// ---- Node ----
// Esta estructura contiene la informacion de un Nodo
//
// Variables miembro
//     - id            : Identificador de un vertice, debe ser irrepetible entre vertices
//     - coord         : La coordenada donde se encuentra el vertice (usado por SFML)
//     - edges         : El conjunto de aristas asociadas al vertice
//     - color         : Color del vertice (usado por SFML)
//     - radius        : Radio del vertice (usado por SFML)
//
// Funciones miembro
//     - parse_csv     : Lee los vertices desde el csv
//     - draw          : Dibuja el vertice instanciado
//     - reset         : Setea 'color' y 'radius' a sus valores por defecto
// *
struct Node {
    std::size_t id;
    sf::Vector2f coord;
    std::vector<Edge *> edges {};

    sf::Color color = default_node_color;
    float radius = default_radius;

    explicit Node(std::size_t id, float x, float y) : id(id), coord(x, y) {}

    static void parse_csv(const std::string &nodes_path, std::map<std::size_t, Node *> &nodes) {
        std::ifstream file(nodes_path);
        std::string id_str, y_str, x_str;

        while (std::getline(file, id_str, ',') &&
               std::getline(file, y_str, ',') &&
               std::getline(file, x_str)) {

            try {
                std::size_t identifier = static_cast<std::size_t>(std::stoll(id_str));
                float x = std::stof(x_str);
                float y = std::stof(y_str);

                nodes.insert({
                    identifier,
                    new Node(identifier, y, x)
                });
            } catch (const std::exception &e) {
                std::cerr << "Error leyendo nodo: " << e.what() << "\n";
            }
               }
    }

    void draw(sf::RenderWindow &window) const {
        sf::CircleShape point(radius);
        point.setPosition(coord);
        point.setFillColor(color);

        window.draw(point);
    }

    void reset() {
        color = default_node_color;
        radius = default_radius;
    }
};


#endif //HOMEWORK_GRAPH_NODE_H
