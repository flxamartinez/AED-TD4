//
// Created by juan-diego on 3/29/24.
//

#ifndef HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H
#define HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H


#include "window_manager.h"
#include "graph.h"
#include <unordered_map>
#include <set>
#include <queue>
#include <cmath>


// Este enum sirve para identificar el algoritmo que el usuario desea simular
enum Algorithm {
    None,
    Dijkstra,
    BFS,
    AStar
};


//* --- PathFindingManager ---
//
// Esta clase sirve para realizar las simulaciones de nuestro grafo.
//
// Variables miembro
//     - path           : Contiene el camino resultante del algoritmo que se desea simular
//     - visited_edges  : Contiene todas las aristas que se visitaron en el algoritmo, notar que 'path'
//                        es un subconjunto de 'visited_edges'.
//     - window_manager : Instancia del manejador de ventana, es utilizado para dibujar cada paso del algoritmo
//     - src            : Nodo incial del que se parte en el algoritmo seleccionado
//     - dest           : Nodo al que se quiere llegar desde 'src'
//*
class PathFindingManager {
    WindowManager *window_manager;
    std::vector<sfLine> path;
    std::vector<sfLine> visited_edges;

    struct Entry {
        Node* node;
        double dist;

        bool operator < (const Entry& other) const {
            return dist < other.dist;
        }
    };

    void dijkstra(Graph &graph) {
        std::unordered_map<Node*, double> dist;
        std::unordered_map<Node*, Node*> parent;
        auto cmp = [&](Node* a, Node* b) { return dist[a] > dist[b]; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> pq(cmp);

        for (auto& [_, node] : graph.nodes) {
            dist[node] = INFINITY;
        }
        dist[src] = 0.0;
        pq.push(src);

        while (!pq.empty()) {
            Node* u = pq.top(); pq.pop();
            if (u == dest) break;

            for (Edge* e : u->edges) {
                Node* v = (e->src == u) ? e->dest : e->src;
                double weight = e->length;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push(v);
                    visited_edges.emplace_back(u->coord, v->coord, sf::Color::Blue, 1.f);
                    render();
                }
            }
        }

        set_final_path(parent);
    }

    void bfs(Graph &graph) {
        std::unordered_map<Node*, Node*> parent;
        std::set<Node*> visited;
        std::queue<Node*> q;

        visited.insert(src);
        q.push(src);

        while (!q.empty()) {
            Node* u = q.front(); q.pop();
            if (u == dest) break;

            for (Edge* e : u->edges) {
                Node* v = (e->src == u) ? e->dest : e->src;
                if (!visited.count(v)) {
                    visited.insert(v);
                    parent[v] = u;
                    q.push(v);
                    visited_edges.emplace_back(u->coord, v->coord, sf::Color::Yellow, 1.f);
                    render();
                }
            }
        }

        set_final_path(parent);
    }


    double heuristic(Node* a, Node* b) {
        float dx = a->coord.x - b->coord.x;
        float dy = a->coord.y - b->coord.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    void a_star(Graph &graph) {
        std::unordered_map<Node*, double> g_score, f_score;
        std::unordered_map<Node*, Node*> parent;
        auto cmp = [&](Node* a, Node* b) { return f_score[a] > f_score[b]; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open_set(cmp);

        for (auto& [_, node] : graph.nodes) {
            g_score[node] = f_score[node] = INFINITY;
        }
        g_score[src] = 0.0;
        f_score[src] = heuristic(src, dest);
        open_set.push(src);

        while (!open_set.empty()) {
            Node* current = open_set.top(); open_set.pop();
            if (current == dest) break;

            for (Edge* e : current->edges) {
                Node* neighbor = (e->src == current) ? e->dest : e->src;
                double tentative_g = g_score[current] + e->length;
                if (tentative_g < g_score[neighbor]) {
                    parent[neighbor] = current;
                    g_score[neighbor] = tentative_g;
                    f_score[neighbor] = tentative_g + heuristic(neighbor, dest);
                    open_set.push(neighbor);
                    visited_edges.emplace_back(current->coord, neighbor->coord, sf::Color::Magenta, 1.f);
                    render();
                }
            }
        }

        set_final_path(parent);
    }


    //* --- render ---
    // En cada iteración de los algoritmos esta función es llamada para dibujar los cambios en el 'window_manager'

    void render() {
        sf::sleep(sf::milliseconds(10));
        window_manager->clear();
        draw(true); // Esto dibuja los nodos, aristas, visitados, y path
        window_manager->display();
    }

    //* --- set_final_path ---
    // Esta función se usa para asignarle un valor a 'this->path' al final de la simulación del algoritmo.
    // 'parent' es un std::unordered_map que recibe un puntero a un vértice y devuelve el vértice anterior a el,
    // formando así el 'path'.
    //
    // ej.
    //     parent(a): b
    //     parent(b): c
    //     parent(c): d
    //     parent(d): NULL
    //
    // Luego, this->path = [Line(a.coord, b.coord), Line(b.coord, c.coord), Line(c.coord, d.coord)]
    //
    // Este path será utilizado para hacer el 'draw()' del 'path' entre 'src' y 'dest'.
    //*
    void set_final_path(std::unordered_map<Node *, Node *> &parent) {
        Node* current = dest;
        while (parent.find(current) != parent.end()) {
            Node* prev = parent[current];
            path.emplace_back(current->coord, prev->coord, sf::Color::Green, 3.f);
            current = prev;
        }
    }

public:
    Node *src = nullptr;
    Node *dest = nullptr;

    explicit PathFindingManager(WindowManager *window_manager) : window_manager(window_manager) {}

    void exec(Graph &graph, Algorithm algorithm) {
        if (src == nullptr || dest == nullptr) return;

        switch (algorithm) {
            case Dijkstra:
                dijkstra(graph);
                break;
            case BFS:
                bfs(graph);
                break;
            case AStar:
                a_star(graph);
                break;
            default:
                break;
        }
    }

    void reset() {
        path.clear();
        visited_edges.clear();

        if (src) {
            src->reset();
            src = nullptr;
            // ^^^ Pierde la referencia luego de restaurarlo a sus valores por defecto
        }
        if (dest) {
            dest->reset();
            dest = nullptr;
            // ^^^ Pierde la referencia luego de restaurarlo a sus valores por defecto
        }
    }

    void draw(bool draw_extra_lines) {
        // Dibujar todas las aristas visitadas
        if (draw_extra_lines) {
            for (sfLine &line: visited_edges) {
                line.draw(window_manager->get_window(), sf::RenderStates::Default);
            }
        }

        // Dibujar el camino resultante entre 'str' y 'dest'
        for (sfLine &line: path) {
            line.draw(window_manager->get_window(), sf::RenderStates::Default);
        }

        // Dibujar el nodo inicial
        if (src != nullptr) {
            src->draw(window_manager->get_window());
        }

        // Dibujar el nodo final
        if (dest != nullptr) {
            dest->draw(window_manager->get_window());
        }
    }
};


#endif //HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H
