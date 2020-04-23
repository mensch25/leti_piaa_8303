#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <set>
#include <map>
#define DBG
using namespace std;

//структура ребра, хранит остаточную пропускную способность и поток, который можно пустить обратно
struct Edge {
    int resC;
    int revF;

    Edge() : resC(0), revF(0) {}
    Edge(int c, int f) : resC(c), revF(f) {}
};

//остаточная сеть
map<char, map<char, Edge>> net;
//сет ребер
set<pair<char, char>> graph;
//вектор посещенных вершин
vector<bool> visited;
char source, sink;

//функция чтение графа из консоли. Заполняет граф и остаточную сеть
void readGraph() {
    int n;
    char u, v;
    int c;

    cin >> n;
    cin >> source >> sink;

    visited.resize(128);

    for (size_t _ = 0; _ < n; _++) {
        cin >> u >> v >> c;
        graph.insert({u, v});
        net[u][v].resC = c;
        if (net.find(v) != net.end() && net[v].find(u) == net[v].end()){
            net[v][u].resC = 0;
        }
    }
}

//рекурсивный поиск в глубину
int dfs(char v, int delta) {
    //если вершина уже была посещена, выходим из нее
    if (visited[v])
        return 0;
    visited[v] = true;

    //если текущая вершина - сток, выходим из нее
    if (v == sink)
        return delta;

    //множество смежных вершин, сортированное по остаточной пропускной способности
    set<pair<int, char>> toVisit;

    for (auto u : net[v]) {
        if (!visited[u.first])
            toVisit.insert({max(u.second.resC, u.second.revF), u.first});
    }
    //обходим вершина из множества в порядке убывания остаточной пропускной способности

    for (auto u = toVisit.rbegin(); u != toVisit.rend(); u++) {
        //если есть поток который можно пустить обратно,
        //находим минимальный вес ребра в пути и делаем это
        if (net[v][u->second].revF > 0) {
            int newDelta = dfs(u->second, min(delta, net[v][u->second].revF));
            if (newDelta > 0) {
                net[u->second][v].resC += newDelta;
                net[v][u->second].revF -= newDelta;
                return newDelta;
            }
        }
        //если остаточная пропускная способность больше нуля,
        //находим минимальный вес ребра в пути и пускаем поток по этому ребру
        if (net[v][u->second].resC > 0) {
            int newDelta = dfs(u->second, min(delta, net[v][u->second].resC));
            if (newDelta > 0) {
                net[u->second][v].revF += newDelta;
                net[v][u->second].resC -= newDelta;
                return newDelta;
            }
        }
    }
    return 0;
}

//функция вывода найденного потока
void print() {
    for (auto &i : graph) {
        cout << i.first << ' ' << i.second << ' ' << net[i.second][i.first].revF;
#ifdef DBG
        cout << "(residual capacity " << net[i.first][i.second].resC
        << ", max capacity " << net[i.second][i.first].revF + net[i.first][i.second].resC << ')';
#endif
        cout << endl;
    }
}

//запуск поиска алгоритмом Форда-Фалкерсона
void findFordFulkerson() {
    int flow = 0;
    int ans = 0;
    while (true) {
        //обнуляем вектор посещенных вершин
        fill(visited.begin(), visited.end(), false);
        //запускаем поиск в глубину
        flow = dfs(source, INT_MAX);
        //если путь не найден - выходим
#ifdef DBG
        cout << "==================\n";
        cout << "current flow = " << ans+flow << endl;
        print();
#endif
        if (flow == 0 || flow == INT_MAX){
#ifdef DBG
            cout << "<!New path not found!>\n";
#endif
            break;
        }
        //обновляем максимальный поток
        ans += flow;
    }
    cout << ans << endl;
    print();
}

int main() {
    readGraph();
    findFordFulkerson();
    return 0;
}

/*
11
a
f
a b 7
a c 3
a d 5
c b 4
c f 5
b f 6
b d 3
b e 4
d b 7
d e 8
e f 10

 */