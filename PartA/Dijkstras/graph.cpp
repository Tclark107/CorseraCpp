#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include <map>

class Graph {
    public:
        Graph(int numVertices, int numEdges);
        void addEdge(int src, int dest);
        void printGraph();
        int getNumEdges();
        int getNumVertices();
        bool isAdjacent(int src, int dest);
        //void getNeighbors(int src); // return a list or just list the  neighbors
                                      // what would we use this for??
        void deleteEdge(int src, int dest);
        //int getVertexValue(int vertex); // why does a vertex have a value, 
                                     // do they mean name??
        //void setVertexValue(int vertex, int value);
        int getEdgeValue(int src, int dest);
        void setEdgeValue(int src, int dest, int value);

        void dijkstra(Graph G);

    private:
        int numVertices;
        int numEdges;
        std::vector<std::list<int>> adjList;
        std::map< std::pair<int,int>, int > edgeValues;
};

Graph::Graph(int numVertices = 0, int numEdges = 0) : numVertices(numVertices), 
    numEdges(numEdges),
    adjList(numVertices),
    edgeValues()
{}

void Graph::addEdge(int src, int dest) {
    adjList[src].push_back(dest);
    adjList[dest].push_back(src);
    numEdges++;
    //should I default edgeValues??
}

void Graph::printGraph() {
    for (int i = 0; i < numVertices; ++i) {
        std::cout << "Adjacency list of vertex " << i << ": ";
        for (int neighbor : adjList[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

int Graph::getNumEdges()
{
    return numEdges;
}

int Graph::getNumVertices()
{
    return numVertices;
}

bool Graph::isAdjacent(int src, int dest)
{
    if(std::find(adjList[src].begin(), adjList[src].end(), dest) != adjList[src].end())
    {
        return true;
    }
    return false;
}

void Graph::deleteEdge(int src, int dest)
{
    if(std::find(adjList[src].begin(), adjList[src].end(), dest) != adjList[src].end())
    {
        adjList[src].remove(dest);
        adjList[dest].remove(src);
        numEdges--;

        std::pair<int,int> edge(src,dest);
        if(edgeValues.find(edge) != edgeValues.end())
        {
            edgeValues.erase(edge);
        }
    }
}

void Graph::setEdgeValue(int src, int dest, int value)
{
    std::pair<int,int> edge(src,dest);
    edgeValues[edge] = value;
}

int Graph::getEdgeValue(int src, int dest)
{
    std::pair<int,int> edge(src,dest);
    if(edgeValues.find(edge) != edgeValues.end())
    {
        return edgeValues[edge];
    }
    std::cout << "No Value Found, returning 0" << std::endl;
    return 0;
}

int Graph::getVertexWithSmallestAvailablePath(const std::map<int,int> unvisited, 
        const std::vector<std::tuple<int,int,int>> shortestPath)
{
    int currentShortestPath = arbitrarilyLargeNumber;
    int vertexWithSmallestPath = 0;
    for(auto const& element : unvisited)
    {
        if(currentShortestPath > get<1>(shortestPath[element.first])
        {
            currentShortestPath = get<1>(shortestPath[element.first]);
            vertexWithSmallestPath = element.first;
        }
    }
        
    if(currentShortestPath == arbitrarilyLargeNumber)
    {
        std::cout << "This function should not return " << currentShortestPath 
                  << std::endl;
        //exit();
    }

    return vertexWithSmallestPath;
}

void Graph::dijkstra()
{
    std::cout << "Computing shortest paths----------" << std::endl;

    int arbitrarilyLargeNumber = 2147483647;

    int length = getNumVertices();
    std:: visited(length);
    std::map<int, int> unvisited(length);

    for(int i = 0; i < length; i++)
    {
        unvisited.insert(i,i);
    }
    
    std::vector<std::tuple<int,int,int>> shortestPath;
    for(int i = 0; i < length, i++)
    {
        shortestPath[i] = make_tuple(i, arbitrarilyLargeNumber, 0);
    }

    while(unvisited.size() > 0) // || we find that some vertices are unvisitable
    {
        int i = getVertexWithSmallestAvailablePath(unvisited, shortestPath);
        std::cout << "vertex with the smallest path is " << i << std::endl;

        int pathToI = get<1>(shortestPath[i]);
        if(pathToI == arbitrarilyLargeNumber)
        {
            std::cout << "Error: pathToI should not be " << pathToI
                 << std::endl;
        }

        for(int j = 0; j < length; j++)
        {
            if(j != i && 
               unvisited.find(j) != unvisited.end())
            {
                if(isAdjacent(i,j))
                {
                    int weight = getEdgeValue(i,j);
                    int currentShortestPath = get<l>(shortestPath[j]);
                    
                    if(pathToI == arbitrarilyLargeNumber)
                    {
                        std::cout << "Error: pathToI should not be " << pathToI
                            << std::endl;
                        std::cout << "Maybe this is the first time we see it, "
                            << "setting to 0" << std::endl;
                        pathToI = 0;
                    }

                    if(currentShortestPath > (weight + pathToI))
                    {
                        get<1>(shortestPath[j]) = weight + pathToI;
                        get<2>(shortestPath[j]) = i;
                    }
                }
            }
        }
        visited.insert(i,i);
        unvisited.erase(i);
    }
}

int main() {
    Graph g(5);
    g.addEdge(0, 1);

    g.setEdgeValue(0, 1, 3);
    g.addEdge(0, 4);
    <g.setEdgeValue(0, 4, 2);
    g.addEdge(1, 2);
    g.setEdgeValue(1, 2, 4);
    g.addEdge(1, 3);
    g.setEdgeValue(1, 3, 7);
    g.addEdge(1, 4);
    g.setEdgeValue(1, 4, 3);
    g.addEdge(2, 3);
    g.setEdgeValue(2, 3, 4);
    g.addEdge(3, 4);
    g.setEdgeValue(3, 4, 1);
    g.printGraph();

    //g.dijkstra();
    
    //std::cout << g.getNumEdges() << std::endl;
    //std::cout << g.getNumVertices() << std::endl;
    //assert(g.isAdjacent(0,1));
    //assert(g.isAdjacent(1,0));
    //assert(!g.isAdjacent(0,5));

    //std::cout << g.isAdjacent(1, 2) << std::endl;
    //std::cout << g.isAdjacent(2, 1) << std::endl;
    //g.deleteEdge(1, 2);
    //std::cout << g.isAdjacent(1, 2) << std::endl;
    //std::cout << g.isAdjacent(2, 1) << std::endl;

    //std::cout << "\n\nCommence Testing Getting and Setting Edge Values\n";
    //std::cout << g.getEdgeValue(0, 4) << std::endl;
    //g.setEdgeValue(0, 4, 10);
    //std::cout << g.getEdgeValue(0, 4) << std::endl;

    //g.setEdgeValue(0, 4, 20);
    //std::cout << g.getEdgeValue(0, 4) << std::endl;
    //
    //g.deleteEdge(0,4);
    //std::cout << g.getEdgeValue(0, 4) << std::endl;


    return 0;
}
