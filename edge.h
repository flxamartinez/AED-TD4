//
// Created by juan-diego on 3/11/24.
//

#ifndef HOMEWORK_GRAPH_EDGE_H
#define HOMEWORK_GRAPH_EDGE_H

#include <SFML/Graphics.hpp>
#include "node.h"
#include <cstring>
#include <fstream>
#include <cmath>
#include <iostream>

// Color por defecto de todas las aristas (usado por SFML)
sf::Color default_edge_color = sf::Color(255, 200, 100);
// Grosor por defecto de todas las aristas (usado por SFML)
float default_thickness = 0.8;


// No tocar esta clase
class sfLine : public sf::Drawable {
public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2, sf::Color color, float thickness)
    : thickness(thickness) {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (this->thickness/2.f)*unitPerpendicular;

        Vertices[0].position = point1 + offset;
        Vertices[1].position = point2 + offset;
        Vertices[2].position = point2 - offset;
        Vertices[3].position = point1 - offset;

        for (auto & vertex : Vertices) {
            vertex.color = color;
        }
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
        target.draw(Vertices,4,sf::Quads);
    }

private:
    sf::Vertex Vertices[4];
    float thickness;
    sf::Color color {};
};


// *
// ---- Edge ----
// Esta estructura contiene la informacion de una arista
//
// Variables miembro
//     - src           : Vértice inicial de la arista
//     - dest          : Vértice final de la arista
//     - max_speed     : Velocidad maxima en la que se puede ir de 'src' a 'dest'
//     - length        : Longitud de la trayectoria de 'src' a 'dest'
//     - one_way       : Si es falso, significa que tambien existe una arista en el grafo de 'dest' a 'src'.
//                       Caso contrario, significa que solo existe la arista actual de 'src' a 'dest'
//     - lanes         : La cantidad de carriles en el camino de 'src' a 'dest'
//     - color         : El color de la linea que conecta a 'src' y 'dest', es usado por SFML
//     - thickness     : El grosor de la linea que conecta a 'src' y 'dest', es usado por SFML
//
// Funciones miembro
//     - parse_csv     : Lee las aristas desde el csv
//     - draw          : Dibuja la arista instanciada
//     - reset         : Setea 'color' y 'thickness' a sus valores por defecto
// *
struct Edge {
    Node *src = nullptr;
    Node *dest = nullptr;
    int max_speed;
    double length;
    bool one_way;
    int lanes;

    sf::Color color = default_edge_color;
    float thickness = default_thickness;

    explicit Edge(Node *src, Node *dest, int max_speed, double length, bool one_way, int lanes) : max_speed(max_speed),
                                                                                                  length(length),
                                                                                                  one_way(one_way),
                                                                                                  lanes(lanes),
                                                                                                  src(src),
                                                                                                  dest(dest) {
    }

    static void parse_csv(const std::string &edges_path, std::vector<Edge *> &edges, std::map<std::size_t, Node *> &nodes) {
        std::ifstream file(edges_path);
        std::string src_str, dest_str, speed_str, length_str, oneway_str, lanes_str;

        while (std::getline(file, src_str, ',') &&
               std::getline(file, dest_str, ',') &&
               std::getline(file, speed_str, ',') &&
               std::getline(file, length_str, ',') &&
               std::getline(file, oneway_str, ',') &&
               std::getline(file, lanes_str)) {

            try {
                std::size_t src_id = std::stoll(src_str);
                std::size_t dest_id = std::stoll(dest_str);

                Node* src_node = nodes.at(src_id);
                Node* dest_node = nodes.at(dest_id);

                Edge* edge = new Edge(
                    src_node,
                    dest_node,
                    std::stoi(speed_str),
                    std::stod(length_str),
                    oneway_str == "True",
                    std::stoi(lanes_str)
                );

                edges.push_back(edge);
            } catch (const std::exception &e) {
                std::cerr << "Error leyendo arista: " << e.what() << "\n";
            }
               }
    }


    void draw(sf::RenderWindow &window) const {
        sfLine line(src->coord, dest->coord, color, thickness);
        line.draw(window, sf::RenderStates::Default);
    }
};


#endif //HOMEWORK_GRAPH_EDGE_H
