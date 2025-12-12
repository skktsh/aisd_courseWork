#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

const int maxVertices = 50;
const int maxEdges = maxVertices * (maxVertices - 1) / 2;
const int maxWeight = 1023;
const int minWeight = 1;

struct Edge {
    string start;
    string end;
    int weight;
};

struct Graph {
    string vertices[maxVertices];
    Edge edges[maxEdges];
    int vertice;
    int edge;
};

struct AdjacencyMatrix {
    int matrix[maxVertices][maxVertices];
    string vertices[maxVertices];
    int vertice;
};

struct AdjacencyList {
    struct Node {
        int weight;
        Node* next;

        string end;
    };

    Node* list[maxVertices];
    string vertices[maxVertices];
    int vertice;
};

struct IncidenceMatrix {
    int matrix[maxVertices][maxEdges];
    string vertices[maxVertices];
    Edge edges[maxEdges];
    int vertice;
    int edge;
};

struct Stack {
    string data[maxVertices];
    int top;

    Stack() {
        top = -1;
    }

    void push(string s) {
        if (top < maxVertices - 1) {
            data[++top] = s;
        }
    }

    string pop() {
        if (top >= 0) {
            return data[top--];
        }
        return "";
    }

    bool isEmpty() {
        return top == -1;
    }
};

struct Queue {
    string data[maxVertices];
    int front;
    int rear;

    Queue() {
        front = 0;
        rear = -1;
    }

    void enqueue(string s) {
        if (rear < maxVertices - 1) {
            data[++rear] = s;
        }
    }

    string dequeue() {
        if (front <= rear) {
            return data[front++];
        }
        return "";
    }

    bool isEmpty() {
        return front > rear;
    }
};

struct DisjointSet {
    int parent[maxVertices];
    int rank[maxVertices];
    int size;
};

Graph* uploadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File could not be opened" << endl;
        return nullptr;
    }

    Graph* graph = new Graph();
    graph->vertice = 0;
    graph->edge = 0;
    string line;

    getline(file, line);
    stringstream ss(line); // input/output
    string vertex;

    while (ss >> vertex) {
        if (graph->vertice >= maxVertices) {
            cout << "Error. Too many vertices.";
            delete graph;
            return 0;
        }

        graph->vertices[graph->vertice++] = vertex;
    }

    for (int i = 0; i < graph->vertice; ++i) {
        getline(file, line);
        stringstream ss2(line);

        for (int j  = 0; j < graph->vertice; ++j) {
            int weight;
            ss2 >> weight;
            if (weight != 0 && j > i) {
                if (graph->edge >= maxEdges) {
                    cout << "Error. Too many edges.";
                    delete graph;
                    return nullptr;
                }
                graph->edges[graph->edge].start = graph->vertices[i];
                graph->edges[graph->edge].end = graph->vertices[j];
                graph->edges[graph->edge].weight = weight;
                ++graph->edge;
            }
        }
    }

    file.close();
    cout << "The amount of Vertices: " << graph->vertice << "; The amount of Edges: " << graph->edge << endl;
    return graph;
}

void printGraph(Graph* graph) {
    if (!graph) {
        cout << "The graph does not exist." << endl;
    }
    cout << "The amount of Vertices: " << graph->vertice << "; The amount of Edges: " << graph->edge << endl;

    for (int i = 0; i < graph->vertice; i++) {
        string vertex = graph->vertices[i];
        cout << vertex << "->";

        bool hasEdges = false;
        for (int j = 0; j < graph->edge; j++) {
            if (graph->edges[j].start == vertex) {
                cout << graph->edges[j].end << "(" << graph->edges[j].weight << ")";
                hasEdges = true;
            }
        }

        if (!hasEdges) {
            cout << "No edges were found.";
        }
        cout << endl;
    }
}

int findIndex(Graph* graph, string vertex) {
    if (!graph) {
        return -1;
    }

    for (int i = 0; i < graph->vertice; i++) {
        if (graph->vertices[i] == vertex) {
            return i;
        }
    }

    return -1;
}

int getNeighbors(Graph* graph, string vertex, string neighbors[]) {
    if (!graph) {
        return -1;
    }

    int count = 0;
    for (int i = 0; i < graph->edge; i++) {
        if (graph->edges[i].start == vertex) {
            neighbors[count++] = graph->edges[i].end;
        }

        else if (graph->edges[i].end == vertex) {
            neighbors[count++] = graph->edges[i].start;
        }

        if (count >= maxVertices) {
            break;
        }
    }

    return count;
}

void breadthTraversal(Graph *graph, string begin) {
    if (!graph) {
        cout << "The graph does not exist." << endl;
        return;
    }

    int beginIndex = findIndex(graph, begin);
    if (beginIndex == -1) {
        cout << "No such vertex was found." << endl;
    }

    bool visited[maxVertices] = {false};
    Queue q;
    string neighbors[maxVertices];

    visited[beginIndex] = true;
    q.enqueue(begin);

    cout << "The starting point of the traversal is " << begin << endl;
    cout << "The Traversal: ";

    while (!q.isEmpty()) {
        string current = q.dequeue();
        cout << current << " ";

        int count = getNeighbors(graph, current, neighbors);
        for (int i = 0; i < count; i++) {
            int index = findIndex(graph, neighbors[i]);
            if (index != - 1 && !visited[index]) {
                visited[index] = true;
                q.enqueue(neighbors[i]);
            }
        }
    }

    cout << endl;
}

void depthTraversal(Graph *graph, string begin) {
    if (!graph) {
        cout << "The graph does not exist." << endl;
        return;
    }

    int beginIndex = findIndex(graph, begin);
    if (beginIndex == -1) {
        cout << "No such vertex was found." << endl;
        return;
    }

    bool visited[maxVertices] = {false};
    Stack stack;
    string neighbors[maxVertices];

    stack.push(begin);
    cout << "The starting point of the traversal is " << begin << endl;
    cout << "The Traversal: ";

    while (!stack.isEmpty()) {
        string current = stack.pop();
        int index = findIndex(graph, current);
        if (index != - 1 && !visited[index]) {
            visited[index] = true;
            cout << current << " ";
            int count = getNeighbors(graph, current, neighbors);

            for (int i = count - 1; i >= 0; i--) {
                int indexNeighbor = findIndex(graph, neighbors[i]);
                if (indexNeighbor != - 1 && !visited[indexNeighbor]) {
                    stack.push(neighbors[i]);
                }
            }
        }
    }

    cout << endl;
}

AdjacencyMatrix* graphToAdjacencyMatrix(Graph* graph) {
    if (!graph) {
        return nullptr;
    }

    AdjacencyMatrix* adjacencyMatrix = new AdjacencyMatrix();
    adjacencyMatrix->vertice = graph->vertice;

    for (int i = 0; i < graph->vertice; i++) {
        adjacencyMatrix->vertices[i] = graph->vertices[i];

        for (int j = 0; j < graph->vertice; j++) {
            adjacencyMatrix->matrix[i][j] = 0;
        }
    }

    for (int k = 0; k < graph->edge; k++) {
        int startIndex = findIndex(graph, graph->edges[k].start);
        int endIndex = findIndex(graph, graph->edges[k].end);

        if (startIndex != -1 && endIndex != -1) {
            adjacencyMatrix->matrix[startIndex][endIndex] = graph->edges[k].weight;
            adjacencyMatrix->matrix[endIndex][startIndex] = graph->edges[k].weight;
        }
    }

    return adjacencyMatrix;
}

void printAdjacencyMatrix(AdjacencyMatrix* adjacencyMatrix) {
    if (!adjacencyMatrix) {
        cout << "The adjacency matrix does not exist." << endl;
        return;
    }

    cout << " ";
    for (int i = 0; i < adjacencyMatrix->vertice; i++) {
        cout << adjacencyMatrix->vertices[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < adjacencyMatrix->vertice; i++) {
        cout << adjacencyMatrix->vertices[i] << " ";
        for (int j = 0; j < adjacencyMatrix->vertice; j++) {
            cout << adjacencyMatrix->matrix[i][j] << " ";
        }
        cout << endl;
    }
}

AdjacencyList* graphToAdjacencyList(Graph* graph) {
    if (!graph) {
        return nullptr;
    }

    AdjacencyList* adjacencyList = new AdjacencyList();
    adjacencyList->vertice = graph->vertice;

    for (int i = 0; i < graph->vertice; i++) {
        adjacencyList->vertices[i] = graph->vertices[i];
        adjacencyList->list[i] = nullptr;
    }

    for (int k = 0; k < graph->edge; k++) {
        int startIndex = findIndex(graph, graph->edges[k].start);
        int endIndex = findIndex(graph, graph->edges[k].end);

        if (startIndex == -1 && endIndex == -1) {
            continue;
        }

        AdjacencyList::Node *node = new AdjacencyList::Node();
        node->end = graph->edges[k].end;
        node->weight = graph->edges[k].weight;
        node->next = adjacencyList->list[startIndex];

        AdjacencyList::Node *node2 = new AdjacencyList::Node();
        node2->end = graph->edges[k].start;
        node2->weight = graph->edges[k].weight;
        node2->next = adjacencyList->list[endIndex];
        adjacencyList->list[endIndex] = node2;
    }

    return adjacencyList;
}

void printAdjacencyList(AdjacencyList* adjacencyList) {
    if (!adjacencyList) {
        cout << "The adjacency list does not exist." << endl;
    }

    for (int i = 0; i < adjacencyList->vertice; i++) {
        cout << adjacencyList->vertices[i] << " -> ";
        AdjacencyList::Node *current = adjacencyList->list[i];

        if (!current) {
            cout << "No edges. " << endl;
        }

        else {
            while (current) {
                cout << current->end << "(" << current->weight << ")";
                current = current->next;
            }
        }
        cout << endl;
    }
}


void deleteAdjacencyList(AdjacencyList* adjacencyList) {
    if (!adjacencyList) {
        return;
    }

    for (int i = 0; i < adjacencyList->vertice; i++) {
        AdjacencyList::Node *current = adjacencyList->list[i];
        while (current) {
            AdjacencyList::Node *temp = current;
            current = current->next;
            delete temp;
        }
    }

    delete adjacencyList;
}

IncidenceMatrix* graphToIncidenceMatrix(Graph* graph) {
    if (!graph) {
        return nullptr;
    }

    IncidenceMatrix* incidenceMatrix = new IncidenceMatrix();
    incidenceMatrix->vertice = graph->vertice;
    incidenceMatrix->edge = graph->edge;

    for (int i = 0; i < graph->vertice; ++i) {
        incidenceMatrix->vertices[i] = graph->vertices[i];
    }

    for (int i = 0; i < graph->edge; ++i) {
        incidenceMatrix->edges[i] = graph->edges[i];
    }

    for (int i = 0; i < incidenceMatrix->vertice; ++i) {
        for (int j = 0; j < incidenceMatrix->edge; ++j) {
            incidenceMatrix->matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < graph->edge; ++i) {
        int startIndex = findIndex(graph, graph->edges[i].start);
        int endIndex = findIndex(graph, graph->edges[i].end);

        if (startIndex != - 1) {
            incidenceMatrix->matrix[startIndex][i] = graph->edges[i].weight;
        }

        if (endIndex != - 1) {
            incidenceMatrix->matrix[endIndex][i] = graph->edges[i].weight;
        }
    }

    return incidenceMatrix;
}

void printIncidenceMatrix(IncidenceMatrix* incidenceMatrix) {
    if (!incidenceMatrix) {
        cout << "The incidence matrix does not exist." << endl;
        return;
    }
    cout << " ";

    for (int i = 0; i < incidenceMatrix->edge; i++) {
        cout << incidenceMatrix->edges[i].start << incidenceMatrix->edges[i].end << " ";
    }
    cout << endl;

    for (int i = 0; i < incidenceMatrix->vertice; i++) {
        cout << incidenceMatrix->vertices[i] << " ";

        for (int j = 0; j < incidenceMatrix->edge; j++) {
            cout << incidenceMatrix->matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

void printGraphTo(Graph* graph) {
    if (!graph) {
        cout << "The graph does not exist." << endl;
        return;
    }

    cout << endl << "Adjacency Matrix:" << endl;
    AdjacencyMatrix* adjacencyMatrix = graphToAdjacencyMatrix(graph);
    printAdjacencyMatrix(adjacencyMatrix);
    delete adjacencyMatrix;

    cout << endl << "Adjacency List:" << endl;
    AdjacencyList* adjacencyList = graphToAdjacencyList(graph);
    printAdjacencyList(adjacencyList);
    deleteAdjacencyList(adjacencyList);

    cout << endl << "Incidence Matrix:" << endl;
    IncidenceMatrix* incidenceMatrix = graphToIncidenceMatrix(graph);
    printIncidenceMatrix(incidenceMatrix);
    delete incidenceMatrix;
}

void makeSet(DisjointSet* set, Graph* graph) {
    if (!set || !graph) {
        return;
    }

    set->size = graph->vertice;
    for (int i = 0; i < graph->vertice; i++) {
        set->parent[i] = i;
        set->rank[i] = 0;
    }
}

int findSet(DisjointSet* set, int value) {
    if (!set) {
        return value;
    }

    if (set->parent[value] != value) {
        set->parent[value] = findSet(set, set->parent[value]);
    }

    return set->parent[value];
}

bool unionSet(DisjointSet* set, int value1, int value2) {
    if (!set) {
        return false;
    }

    int rootValue1 = findSet(set, value1);
    int rootValue2 = findSet(set, value2);

    if (rootValue1 == rootValue2) {
        return false;
    }

    if (set->rank[rootValue1] > set->rank[rootValue2]) {
        set->parent[rootValue2] = rootValue1;
    }

    else if (set->rank[rootValue1] < set->rank[rootValue2]) {
        set->parent[rootValue1] = rootValue2;
    }

    else {
        set->parent[rootValue2] = rootValue1;
        set->rank[rootValue1]++;
    }

    return true;
}

void insertionSort(Graph* graph) {
    for (int i = 0; i < graph->edge - 1; i++) {
        for (int j = 0; j < graph->edge - i - 1; j++) {
            if (graph->edges[j].weight > graph->edges[j + 1].weight) {
                Edge temp = graph->edges[j];
                graph->edges[j] = graph->edges[j + 1];
                graph->edges[j + 1] = temp;
            }
        }
    }
}

void kruskalAlgorithm(Graph* graph) {
    cout << "Kruskal's Algorithm:" << endl;

    if (!graph) {
        cout << "The graph does not exist." << endl;
        return;
    }

    if (graph->edge == 0) {
        cout << "There are no endes in the graph." << endl;
        return;
    }

    insertionSort(graph);
    Edge sortedEdges[maxEdges];
    for (int i = 0; i < graph->edge; i++) {
        sortedEdges[i] = graph->edges[i];
    }

    DisjointSet set;
    makeSet(&set, graph);

    Edge result[maxVertices];
    int resultSize = 0;
    int weight = 0;

    cout << "Sorted Edges:" << endl;
    for (int i = 0; i < graph->edge; i++) {
        cout << graph->edges[i].start << " - " << graph->edges[i].end << "(" << graph->edges[i].weight << ")" << endl;
    }

    cout << "Minimum Spanning Tree search: " << endl;
    for (int i = 0; i < graph->edge && resultSize < graph->vertice; i++) {
        int startIndex = findIndex(graph, sortedEdges[i].start);
        int endIndex = findIndex(graph, sortedEdges[i].end);

        if (findSet(&set, startIndex) != findSet(&set, endIndex)) {
            if (sortedEdges[i].start > sortedEdges[i].end) {
                swap(sortedEdges[i].start, sortedEdges[i].end);
            }

            result[resultSize++] = sortedEdges[i];
            weight += sortedEdges[i].weight;
            unionSet(&set, startIndex, endIndex);

            cout << "Adding an edge: " << graph->edges[i].start << " " << graph->edges[i].end << "(weight: " << graph->edges[i].weight << ")" << endl;
        }
    }

    if (resultSize < graph->vertice - 1) {
        cout << "Graph is not connected. " << endl;
        return;
    }

    for (int i = 0; i < resultSize - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < resultSize; j++) {
            if (result[j].start < result[minIndex].start || (result[j].start == result[minIndex].start && result[j].end < result[minIndex].end)) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swap(result[i], result[minIndex]);
        }
    }

    cout << "Minimum Spanning Tree: " << endl;
    cout << "Edges: " << endl;
    for (int i = 0; i < resultSize; i++) {
        cout << result[i].start << " " << result[i].end << endl;
    }
    cout << "Total weight: " << weight << endl;
}

void menu() {
    cout << "1. Upload a graph from a file." << endl;
    cout << "2. To sort edges" << endl;
    cout << "3. Print the existing graph" << endl;
    cout << "4. Breadth Traversal" << endl;
    cout << "5. Depth Traversal" << endl;
    cout << "6. Kruskals' Algorithm" << endl;
    cout << "7. Adjacency Matrix, Adjacency List, Incidence Matrix" << endl;
    cout << "0. Exit" << endl;
}

int main() {
    Graph* graph = nullptr;
    int choice;
    string filename;

    while (true) {
        menu();

        cout << "Enter your choice: ";
        cout << endl;
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter the file name: ";
                cin >> filename;

                if (graph) {
                    delete graph;
                }
                graph = uploadFromFile(filename);
                break;

            case 2:
                if (graph) {
                    cout << "The Graph before sorting its edges: " << endl;
                    for (int i = 0; i < graph->edge; i++) {
                        cout << graph->edges[i].start << " - " << graph->edges[i].end << "(" << graph->edges[i].weight << ")" << endl;
                    }

                    insertionSort(graph);
                    cout << "The Graph after sorting its edges: " << endl;
                    for (int i = 0; i < graph->edge; i++) {
                        cout << graph->edges[i].start << " - " << graph->edges[i].end << "(" << graph->edges[i].weight << ")" << endl;
                    }
                }

                else {
                    cout << "The Graph does not exist." << endl;
                }
                break;

            case 3:
                printGraph(graph);
                break;

            case 4:
               if (graph) {
                   string begin;
                   cout << "Enter the vertice to start from: ";
                   cin >> begin;
                   breadthTraversal(graph, begin);
               }

                else {
                    cout << "The Graph does not exist." << endl;
                }
                break;

            case 5:
                if (graph) {
                    string begin;
                    cout << "Enter the vertice to start from: ";
                    cin >> begin;
                    depthTraversal(graph, begin);
                }

                else {
                    cout << "The Graph does not exist." << endl;
                }
                break;

            case 6:
                kruskalAlgorithm(graph);
                break;

            case 7:
                printGraphTo(graph);
                break;

            case 0:
                return 0;

            default:
                cout << "Incorrect choice" << endl;
        }
    }
}