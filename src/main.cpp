#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <cfloat>
#include <queue>
//#define GREEDY        //Жадный алгоритм
#define ASTAR           //Алгоритм A*
using namespace std;

map <char, vector<pair<char, double>>> graph; //контейнер для хранения графа
map <char, bool> visited;   //контейнер для посещенных вершин для жадного алгоритма


//функция компаратор для очереди с приоритетом
bool compare (pair<char, double> a, pair<char, double> b){
    if (a.second == b.second)
        return a.first < b.first;
    return a.second > b.second;
}

//Поиск пути жадным алгоритмом
string greedySearchWay(char start, char end){
    cout << "\nGREEDY\n";
    double min;
    string res; //строка для хранения пути
    res += start;
    char curPeak = start; //выбираем start в качестве начальной вершины
    while (curPeak != end){ //запускаем цикл пока не найдем конечную вершину
        cout << res << '\n';
        char nextPeak;
        min = DBL_MAX;
        bool find = false;
        //проходим по всем смежным вершинам и выбираем вершину, ребро с которой имеет минимальный вес
        for (auto & i : graph[curPeak]){
            if (i.second < min && !visited[i.first]){
                min = i.second;
                nextPeak = i.first;
                find = true;
            }
        }
        //если у текущей вершины нет смежных или они все посещены, откатываемся и  переходим к следующей
        if (!find){
            visited[curPeak] = true;
            if (!res.empty()) {
                res.pop_back();
                curPeak = res[res.size() - 1];
                cout << "return\n";
            }
            else return "Path not found\n";
            continue;
        }
        visited[curPeak] = true;
        curPeak = nextPeak;
        res += curPeak;
    }
    return "result: " + res;
}

string aStarSearchWay(char start, char end, int n = 1){
    cout << "\nASTAR\n";
    map<char, pair<string, double>> ways; //структура хранящая минимальные пути до всех вершин
    priority_queue<pair<char, double>, vector<pair<char, double>>, decltype(&compare)> priorityQueue(&compare); //очередь с приоритетом
    priorityQueue.push({start, abs(end-start)}); //инициализируем очередь начальной вершиной
    ways[start].first = start; //путь до начальной вершины содержит только ее саму
    while (!priorityQueue.empty()){ //выполняем цикл пока очередь не пуста
        vector<pair<char, double>> peaks; //структура хранящая вершины, обрабатываемые на этом шаге
        if (priorityQueue.top().first == end) //если в очереди самый приоритетный элемент это конечная вершина, значит мы нашли нужный путь
            return ways[end].first;
        cout << "\nQueue condition: \n";
        priority_queue<pair<char, double>, vector<pair<char, double>>, decltype(&compare)> priorityQueueCopy(priorityQueue);
        while (!priorityQueueCopy.empty()){
            cout << priorityQueueCopy.top().first << "(" << priorityQueueCopy.top().second << ") ";
            priorityQueueCopy.pop();
        }
        cout << "\nPeaks processed in this step: ";
        bool popEnd = false;
        pair<char,double> endFind;
        for (int i = 0; i < n && !priorityQueue.empty(); i++){ //снимаем из очереди n вершин, или сколько есть, если очередь кончится быстрее
            auto peak = priorityQueue.top();
            priorityQueue.pop();
            if (peak.first == end){
                endFind = peak;
                popEnd = true;
                i--;
                continue;
            }
            peaks.push_back(peak);
            cout << peak.first << ' ';
        }

        if (popEnd)
            priorityQueue.push(endFind);

        cout << '\n';
        for (auto & curPeak : peaks) { //обрабатываем снятые вершины: добавляем в очередь смежные с ними вершинами
            if (graph[curPeak.first].empty()) {
                continue;
            }
            for (auto &i : graph[curPeak.first]) {
                double euristic = abs(end - i.first); //считаем эвристику для каждой смежной вершины
                //если найденный путь лучше найденного ранее или ранее вообще не было найдено пути, записываем текущий путь
                if (ways[i.first].second > ways[curPeak.first].second + i.second || ways[i.first].second == 0) {
                    ways[i.first].second = ways[curPeak.first].second + i.second;
                    ways[i.first].first = ways[curPeak.first].first + i.first;
                    cout << "New path to " << i.first << " : " << ways[i.first].first << " (" << ways[i.first].second << ")\n";
                }
                priorityQueue.push({i.first, euristic + ways[i.first].second}); //добавляем вершину в очередь
            }
        }

    }

    return ways[end].first;
}

int main() {
    char start, end;
    int n = 1;
#ifdef ASTAR
    cin >> n;
#endif
    cin >> start >> end;
    char from, to;

    while (cin >> from && from!='#'){
        double weight;
        cin >> to >> weight;
        graph[from].push_back({to, weight});
        visited[from] = false;
        visited[to] = false;
    }
#ifdef GREEDY
    cout << greedySearchWay(start, end);
    for (auto & i : visited)
        i.second = false;
#endif

#ifdef ASTAR
    string res = aStarSearchWay(start, end, n);
    if (res == "")
        res = "Path not found";
    cout << "\nresult: " << res;
#endif
    return 0;
}
/*
3
a e
e b 1.0
a d 1.49
b c 1.0
c f 2.0
f g 1.0
a b 1.0
d e 1.39
e g 2.0
#
4
a e
a b 3.2
d c 1.0
b g 4.0
b o 5.0
b v 2.3
d e 2.3
c f 1.0
f g 1.0
f e 3.0
e g 2.0
#

*/

