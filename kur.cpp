#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph {
public:
    int vertices;
    vector<vector<pair<int, int>>> adj; // Список смежности (вес, вершина)

    Graph(int v) : vertices(v) {
        adj.resize(v);
    }

    void addEdge(int u, int v, int weight) {
        adj[u].emplace_back(weight, v);
    }

    void bellmanFord(int source) {
        vector<int> distance(vertices, numeric_limits<int>::max()); //всем вершинам кроме нач присваивается бесконечное расстояние
        distance[source] = 0; //вектор хранит мин расстояние от нач вершины до остальных, расстояние от нач до самого себя = 0

        for (int i = 1; i < vertices; i++) { //цикл выполняется 'кол-во вершин - 1' раз
            for (int u = 0; u < vertices; u++) {  //проход по каждой вершине графа
                for (auto& edge : adj[u]) { //проход по всем исходящим ребрам из текущей вершины u
                    int weight = edge.first; //запись веса ребра
                    int v = edge.second; //запись конечной вершины
                    if (distance[u] != numeric_limits<int>::max() && distance[u] + weight < distance[v]) { //релаксация
                        distance[v] = distance[u] + weight;                                                //проверяет если расстояние до вершины u найдено (не бесконечно)
                    }                                                                                      //и если путь через вершину u до v короче чем текущее хранящееся в distance
                }                                                                                          //то расстояние до v обновляется
            }
        }

        // Проверка на наличие отрицательных циклов
        for (int u = 0; u < vertices; u++) {
            for (auto& edge : adj[u]) {
                int weight = edge.first;
                int v = edge.second;
                if (distance[u] != numeric_limits<int>::max() && distance[u] + weight < distance[v]) {
                    cout << "Граф содержит отрицательный цикл!" << endl;
                    return;
                }
            }
        }

        // Вывод расстояний
        cout << "Расстояния от источника " << source << ":" << endl;
        for (int i = 0; i < vertices; i++) {
            if (distance[i] == numeric_limits<int>::max()) {
                cout << "До вершины " << i << " нет пути." << endl;
            }
            else {
                cout << "Расстояние до вершины " << i << " равно " << distance[i] << endl;
            }
        }
    }

    void printGraph() {
        cout << "Граф (вес, вершина):" << endl;
        for (int u = 0; u < vertices; u++) {
            cout << "Вершина " << u << ": ";
            if (adj[u].empty()) {
                cout << "нет рёбер.";
            }
            else {
                for (auto& edge : adj[u]) {
                    cout << "(" << edge.first << ", " << edge.second << ") ";
                }
            }
            cout << endl;
        }
    }
};

void generateRandomGraph(Graph& graph, int edges) {
    // Сначала создаем минимальное количество рёбер для связности
    for (int i = 0; i < graph.vertices - 1; i++) {
        int weight = rand() % 21 - 10; // Случайный вес от -10 до 10
        graph.addEdge(i, i + 1, weight);
    }

    // Теперь добавляем дополнительные рёбра
    int addedEdges = graph.vertices - 1; // Учитываем уже добавленные рёбра
    while (addedEdges < edges) {
        int u = rand() % graph.vertices;
        int v = rand() % graph.vertices;
        if (u != v) {
            int weight = rand() % 21 - 10; // Случайный вес от -10 до 10
            bool edgeExists = false;
            for (const auto& edge : graph.adj[u]) {
                if (edge.second == v) {
                    edgeExists = true;
                    break;
                }
            }
            if (!edgeExists) {
                graph.addEdge(u, v, weight);
                addedEdges++;
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел

    int vertices;
    while (true) {
        cout << "Введите количество вершин в графе (положительное число): ";
        cin >> vertices;
        if (cin.fail() || vertices <= 0) {
            cin.clear(); // Очистка флага ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорирование оставшегося ввода
            cout << "Неверный ввод. Попробуйте снова." << endl;
        }
        else {
            break;
        }
    }

    Graph graph(vertices);

    char choice;
    while (true) {
        cout << "Хотите ввести рёбра вручную (h) или сгенерировать случайный граф (r)? ";
        cin >> choice;
        if (choice == 'h' || choice == 'r') {
            break;
        }
        else {
            cout << "Неверный выбор! Пожалуйста, введите 'h' или 'r'." << endl;
        }
    }

    if (choice == 'h') {
        int edges;
        while (true) {
            cout << "Введите количество рёбер (положительное число): ";
            cin >> edges;
            if (cin.fail() || edges <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Неверный ввод. Попробуйте снова." << endl;
            }
            else {
                break;
            }
        }

        cout << "Введите рёбра в формате 'u v weight' (по одному на строку):" << endl;
        for (int i = 0; i < edges; i++) {
            int u, v, weight;
            while (true) {
                cout << "Ребро " << (i + 1) << ": ";
                cin >> u >> v >> weight;
                if (cin.fail() || u < 0 || u >= vertices || v < 0 || v >= vertices) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Неверный ввод. Убедитесь, что вершины находятся в диапазоне [0, " << vertices - 1 << "]." << endl;
                }
                else {
                    graph.addEdge(u, v, weight); // Теперь вес может быть отрицательным
                    break;
                }
            }
        }
    }
    else if (choice == 'r') {
        int edges;
        while (true) {
            cout << "Введите количество рёбер для случайной генерации (положительное число): ";
            cin >> edges;
            if (cin.fail() || edges <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Неверный ввод. Попробуйте снова." << endl;
            }
            else {
                generateRandomGraph(graph, edges);
                graph.printGraph(); // Вывод графа после случайной генерации
                break;
            }
        }
    }

    int source;
    while (true) {
        cout << "Введите источник (номер вершины от 0 до " << vertices - 1 << "): ";
        cin >> source;
        if (cin.fail() || source < 0 || source >= vertices) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Убедитесь, что источник находится в диапазоне [0, " << vertices - 1 << "]." << endl;
        }
        else {
            break;
        }
    }

    graph.bellmanFord(source);

    return 0;
}