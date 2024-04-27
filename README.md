# Tarea de Grafos

El objetivo de esta tarea es implementar un **Path Finder** para la ciudad de Lima. 

## Dependencias

Para esta tarea se solicita utilizar ```C++17``` y la librería ```SFML 2.5```

- Para instalar ```SFML 2.5```:

    - [Windows](https://www.youtube.com/watch?v=HkPRG0vfObc)
    - [MacOS y Linux](https://www.youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua)

Cuando se instale la librería, probar que las siguientes líneas del ```CMakeLists.txt``` encuentren la librería adecuadamente.
```cmake
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window)
else()
    message("SFML not found")
endif()
```

## Dataset
El dataset consiste de dos csv:

- *nodes.csv*
    

- *edges.csv*

## Algoritmos
Se les solicita implementar 2 algoritmos para busqueda en grafos

- *Dijkstra*


- *A**


## Diagrama de clases UML 
![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/a5788651-92d1-4304-b611-6347bfb08aa5)
