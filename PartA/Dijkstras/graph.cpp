#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include <map>
#include <random>

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
{
}

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
    std::pair<int,int> reverse(dest,src);
    edgeValues[reverse] = value;
}

int Graph::getEdgeValue(int src, int dest)
{
    std::pair<int,int> edge(src,dest);
    if(edgeValues.find(edge) != edgeValues.end())
    {
        return edgeValues[edge];
    }
    std::cout << "No Value Found, returning 0" << std::endl;
    std::cout << "The src is " << src << " and the dest is " 
        << dest << std::endl;
    return 0;
}


class ShortestPath
{
    public:
        ShortestPath(Graph G);
        void dijkstra(Graph G);
        void printShortestPaths();
        float getAveragePath(Graph G);

    private:
        int arbitrarilyLargeNumber;
        std::map<int, int> visited;
        std::map<int, int> unvisited;
        std::vector<std::tuple<int,int,int>> shortestPath;
        int getVertexWithSmallestAvailablePath(const std::map<int,int> unvisited,
                                                const std::vector<std::tuple<int,int,int>>);
        void initializeShortestPath(int length);
        void initializeUnvisited(int length);
};

ShortestPath::ShortestPath(Graph G) :
    arbitrarilyLargeNumber(2147483647)
{
    int length = G.getNumVertices();
    initializeShortestPath(length);
    initializeUnvisited(length);
}

void ShortestPath::initializeShortestPath(int length)
{
    shortestPath.resize(length);
    for(int i = 0; i < length; i++)
    {
        std::get<0>(shortestPath[i]) = i;
        std::get<1>(shortestPath[i]) = arbitrarilyLargeNumber;
        std::get<2>(shortestPath[i]) = 0;
    }
}

void ShortestPath::initializeUnvisited(int length)
{
    for(int i = 0; i < length; i++)
    {
        unvisited.insert({i,i});
    }
}

int ShortestPath::getVertexWithSmallestAvailablePath(const std::map<int,int> unvisited, 
        const std::vector<std::tuple<int,int,int>> shortestPath)
{
    int currentShortestPath = arbitrarilyLargeNumber;
    
    auto it = unvisited.begin();
    int vertexWithSmallestPath = it->first;

    while(it != unvisited.end())
    {
        if(currentShortestPath > std::get<1>(shortestPath[it->first]))
        {
            currentShortestPath = std::get<1>(shortestPath[it->first]);
            vertexWithSmallestPath = it->first;
        }
        it++;
    }
        
    if(currentShortestPath == arbitrarilyLargeNumber)
    {
        std::cout << "This function should not return " << currentShortestPath 
                  << std::endl;
        //exit();
    }

    return vertexWithSmallestPath;
}

void ShortestPath::dijkstra(Graph G)
{
    std::cout << "Computing shortest paths----------" << std::endl;

    int length = G.getNumVertices();
    int count = 0;
    while(count < length) 
    {
        int i = getVertexWithSmallestAvailablePath(unvisited, shortestPath);
        std::cout << "vertex with the smallest path is " << i << std::endl;
        std::cout << "the smallest path value is " << std::get<1>(shortestPath[i])
            << std::endl;

        int pathToI = std::get<1>(shortestPath[i]);
        if(pathToI == arbitrarilyLargeNumber)
        {
            std::cout << "Error: pathToI should not be " << pathToI
                 << ", unless this is the first vertex we are processing "
                 << "or the vertex is unvisitble. " 
                 << std::endl;
        }

        for(int j = 0; j < length; j++)
        {
            if(j != i && 
               unvisited.find(j) != unvisited.end())
            {
                if(G.isAdjacent(i,j))
                {
                    int weight = G.getEdgeValue(i,j);
                    int currentShortestPath = std::get<1>(shortestPath[j]);
                    
                    if(pathToI == arbitrarilyLargeNumber)
                    {
                        std::cout << "This should never be the case unless its the first one"
                            << std::endl;
                        std::cout << "Error: pathToI should not be " << pathToI
                            << std::endl;
                        std::cout << "Maybe this is the first time we see it, "
                            << "setting to 0" << std::endl;
                        std::cout << "we are; dealing with vertex: " << i 
                            << " and vertex " << j 
                            << "the current shortest path is " <<  std::get<1>(shortestPath[j])
                            << " the new weight is " << weight
                            << std::endl;
                        pathToI = 0;
                    }

                    if(currentShortestPath > (weight + pathToI))
                    {
                        std::get<1>(shortestPath[j]) = weight + pathToI;
                        std::get<2>(shortestPath[j]) = i;
                    }
                }
            }
        }
        visited.insert({i,i});
        unvisited.erase(i);
        count++;
    }
    if(count >= length)
    {
        std::cout << "We have processed the entire visited list and "
            << " all shortest paths should be found" 
            << std::endl;
    }
}


void ShortestPath::printShortestPaths()
{
    int len = shortestPath.size();
    for(int i = 0; i < len; i++)
    {
        std::cout << "The shortest path for vertex " << i 
                << " from 0 is " << std::get<1>(shortestPath[i])
                << std::endl;
    }
}

class MonteCarlo
{
    public:
        MonteCarlo();
        Graph generateRandomGraph(int size, float density);

    private:
        int generateEdgeValue();

};

MonteCarlo::MonteCarlo()
{
}

Graph MonteCarlo::generateRandomGraph(int size, float density)
{

    bool** graph;
    srand(time(0));
    graph = new bool*[size];
    for(int i = 0; i < size; i++)
    {
        graph[i] = new bool[size];
    }

    Graph G(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    
    for(int i = 0; i < size; i++)
    {
        for(int j = i; j < size; j++)
        {
            if(i == j) 
            {
                graph[i][j] = false;
            }
            else
            {
                float randomValue = dist(gen);
                std::cout << "random valu = " << randomValue << std::endl;

                graph[i][j] = graph[j][i] = (randomValue < density);
                std::cout << "graphij = " << graph[i][j] << std::endl;
                if(graph[i][j])
                {
                    int edgeValue = generateEdgeValue();
                    std::cout << "Add an edge between vertex " << i
                        << " and vertex " << j 
                        << " and it has a value of " << edgeValue
                        << std::endl;

                    G.addEdge(i, j);
                    G.setEdgeValue(i, j, edgeValue);
                }
            }
        }
    }
    return G;
}

int MonteCarlo::generateEdgeValue()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(2, 10);

    int randomNumber = dist(gen);
    return randomNumber;
}
 

float ShortestPath::getAveragePath(Graph G)
{
    int sum = 0;
    std::cout << "size of graph is " <<G.getNumVertices()  << std::endl;
    for(int i = 0; i < G.getNumVertices(); i++)
    {
        if(std::get<1>(shortestPath[i]) == 2147483647)
        {
            std::get<1>(shortestPath[i]) = 0;
        }

        std::cout << "shortestpath at " << i << " is " << std::get<1>(shortestPath[i]);
        sum += std::get<1>(shortestPath[i]);
        std::cout << std::endl;
        std::cout << "sum is now " << sum << std::endl;
    }
    std::cout << std::endl;
    float result = static_cast<float>(sum)/G.getNumVertices(); 
    std::cout << "result = " << result << std::endl;
    return result;
}

int main() {
    //Graph g(5);
    //g.addEdge(0, 1);
    //g.setEdgeValue(0, 1, 3);
    //g.addEdge(0, 4);
    //g.setEdgeValue(0, 4, 2);
    //g.addEdge(1, 2);
    //g.setEdgeValue(1, 2, 4);
    //g.addEdge(1, 3);
    //g.setEdgeValue(1, 3, 7);
    //g.addEdge(1, 4);
    //g.setEdgeValue(1, 4, 3);
    //g.addEdge(2, 3);
    //g.setEdgeValue(2, 3, 4);
    //g.addEdge(3, 4);
    //g.setEdgeValue(3, 4, 1);
    //g.printGraph();

    //ShortestPath sP(g);
    //sP.dijkstra(g);
    //sP.printShortestPaths();

    MonteCarlo mC;
    Graph a = mC.generateRandomGraph(50, 0.10);
    a.printGraph();

    ShortestPath sP(a);
    sP.dijkstra(a);
    sP.printShortestPaths();

    int count = 100;
    std::vector<float> avgPath;
    for(int i = 0; i < count; i++)
    {
        MonteCarlo blah;
        Graph temp = blah.generateRandomGraph(1000, 0.2);
        temp.printGraph();

        ShortestPath foo(temp);
        foo.dijkstra(temp);
        foo.printShortestPaths();
        float avg = foo.getAveragePath(temp);
        std::cout << "avg = " << avg << std::endl;
        avgPath.push_back(avg);
    }

    std::cout << "The avg paths for " << avgPath.size() << "graphs are: " ;
    for(int i = 0; i < count; i++)
    {
        std::cout << avgPath[i] << std::endl;
    }

    std::cout << "The average of the averages is ";
    float sum = 0;
    for(int i = 0; i < avgPath.size(); i++)
    {
        sum += avgPath[i];
    }
    float result = sum/avgPath.size();
    std::cout << result << std::endl;


    
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
