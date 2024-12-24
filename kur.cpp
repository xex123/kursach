#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

class Graph {
public:
    int vertices;
    vector<vector<pair<int, int>>> adj; // ������ ��������� (���, �������)

    Graph(int v) : vertices(v) {
        adj.resize(v);
    }

    void addEdge(int u, int v, int weight) {
        adj[u].emplace_back(weight, v);
    }

    void bellmanFord(int source) {
        vector<int> distance(vertices, numeric_limits<int>::max()); //���� �������� ����� ��� ������������� ����������� ����������
        distance[source] = 0; //������ ������ ��� ���������� �� ��� ������� �� ���������, ���������� �� ��� �� ������ ���� = 0

        for (int i = 1; i < vertices; i++) { //���� ����������� '���-�� ������ - 1' ���
            for (int u = 0; u < vertices; u++) {  //������ �� ������ ������� �����
                for (auto& edge : adj[u]) { //������ �� ���� ��������� ������ �� ������� ������� u
                    int weight = edge.first; //������ ���� �����
                    int v = edge.second; //������ �������� �������
                    if (distance[u] != numeric_limits<int>::max() && distance[u] + weight < distance[v]) { //����������
                        distance[v] = distance[u] + weight;                                                //��������� ���� ���������� �� ������� u ������� (�� ����������)
                    }                                                                                      //� ���� ���� ����� ������� u �� v ������ ��� ������� ���������� � distance
                }                                                                                          //�� ���������� �� v �����������
            }
        }

        // �������� �� ������� ������������� ������
        for (int u = 0; u < vertices; u++) {
            for (auto& edge : adj[u]) {
                int weight = edge.first;
                int v = edge.second;
                if (distance[u] != numeric_limits<int>::max() && distance[u] + weight < distance[v]) {
                    cout << "���� �������� ������������� ����!" << endl;
                    return;
                }
            }
        }

        // ����� ����������
        cout << "���������� �� ��������� " << source << ":" << endl;
        for (int i = 0; i < vertices; i++) {
            if (distance[i] == numeric_limits<int>::max()) {
                cout << "�� ������� " << i << " ��� ����." << endl;
            }
            else {
                cout << "���������� �� ������� " << i << " ����� " << distance[i] << endl;
            }
        }
    }

    void printGraph() {
        cout << "���� (���, �������):" << endl;
        for (int u = 0; u < vertices; u++) {
            cout << "������� " << u << ": ";
            if (adj[u].empty()) {
                cout << "��� ����.";
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
    // ������� ������� ����������� ���������� ���� ��� ���������
    for (int i = 0; i < graph.vertices - 1; i++) {
        int weight = rand() % 21 - 10; // ��������� ��� �� -10 �� 10
        graph.addEdge(i, i + 1, weight);
    }

    // ������ ��������� �������������� ����
    int addedEdges = graph.vertices - 1; // ��������� ��� ����������� ����
    while (addedEdges < edges) {
        int u = rand() % graph.vertices;
        int v = rand() % graph.vertices;
        if (u != v) {
            int weight = rand() % 21 - 10; // ��������� ��� �� -10 �� 10
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
    srand(static_cast<unsigned int>(time(0))); // ������������� ���������� ��������� �����

    int vertices;
    while (true) {
        cout << "������� ���������� ������ � ����� (������������� �����): ";
        cin >> vertices;
        if (cin.fail() || vertices <= 0) {
            cin.clear(); // ������� ����� ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������������� ����������� �����
            cout << "�������� ����. ���������� �����." << endl;
        }
        else {
            break;
        }
    }

    Graph graph(vertices);

    char choice;
    while (true) {
        cout << "������ ������ ���� ������� (h) ��� ������������� ��������� ���� (r)? ";
        cin >> choice;
        if (choice == 'h' || choice == 'r') {
            break;
        }
        else {
            cout << "�������� �����! ����������, ������� 'h' ��� 'r'." << endl;
        }
    }

    if (choice == 'h') {
        int edges;
        while (true) {
            cout << "������� ���������� ���� (������������� �����): ";
            cin >> edges;
            if (cin.fail() || edges <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "�������� ����. ���������� �����." << endl;
            }
            else {
                break;
            }
        }

        cout << "������� ���� � ������� 'u v weight' (�� ������ �� ������):" << endl;
        for (int i = 0; i < edges; i++) {
            int u, v, weight;
            while (true) {
                cout << "����� " << (i + 1) << ": ";
                cin >> u >> v >> weight;
                if (cin.fail() || u < 0 || u >= vertices || v < 0 || v >= vertices) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "�������� ����. ���������, ��� ������� ��������� � ��������� [0, " << vertices - 1 << "]." << endl;
                }
                else {
                    graph.addEdge(u, v, weight); // ������ ��� ����� ���� �������������
                    break;
                }
            }
        }
    }
    else if (choice == 'r') {
        int edges;
        while (true) {
            cout << "������� ���������� ���� ��� ��������� ��������� (������������� �����): ";
            cin >> edges;
            if (cin.fail() || edges <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "�������� ����. ���������� �����." << endl;
            }
            else {
                generateRandomGraph(graph, edges);
                graph.printGraph(); // ����� ����� ����� ��������� ���������
                break;
            }
        }
    }

    int source;
    while (true) {
        cout << "������� �������� (����� ������� �� 0 �� " << vertices - 1 << "): ";
        cin >> source;
        if (cin.fail() || source < 0 || source >= vertices) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����. ���������, ��� �������� ��������� � ��������� [0, " << vertices - 1 << "]." << endl;
        }
        else {
            break;
        }
    }

    graph.bellmanFord(source);

    return 0;
}