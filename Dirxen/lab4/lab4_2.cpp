#include <iostream>
#include <string>
#include <vector>
//#define DBG
using namespace std;

//функция подсчета префиксной функции для строки
vector<int> prefix(string str){
    //создаем вектор необходимого размера
    vector<int> p(str.size() + 1, 0);
    p[0] = 0;
    for (int i = 1; i <= str.size(); i++) {
        //начинаем проверять равенство символов начиная с символа под номером k,
        //который равен префиксной функции на предыдущем шаге
        int k = p[i - 1];
        //проверяем до тех пор пока символы не совпадут либо k не станет равно 0
        while (k > 0 && str[k] != str[i])
            k = p[k - 1];
        //если мы вышли по условию совпадения символов увеличим k, после этого запишем
        //значение префиксной функции в вектор
        if (str[i] == str[k])
            k++;
        p[i] = k;
    }
    return p;
}

int findLoop(string A, string B){
    //если длины строк не равны сразу возвращаем -1
    if (A.size() != B.size())
        return -1;
    //считаем префиксную функцию для шаблона
    vector<int> p = prefix(A);
#ifdef DBG
    cout << "Prefix function of " << A << " : ";
    for (int i : p)
        cout << i << ' ';
    cout << endl;
#endif
    vector<int> res;
    int m = A.size();
    int k = 0;
    //начинаем проверять равенство символов начиная с нулевого символа и проходим строку два раза
    for (int i = 0; i < 2*B.size(); i++) {
        while (k > 0 && A[k] != B[i%B.size()])
            k = p[k - 1];
        //если мы вышли по условию совпадения символов увеличим k, но в вектор уже ничего не добавляем
        if (A[k] == B[i%B.size()])
            k++;
        if (i >= B.size() && k == 0){
            return -1;
        }
#ifdef DBG
        const std::string green("\033[0;32m");
        const std::string reset("\033[0m");
        int st = max(0, (int)(i-B.size())+1);
        cout << "Prefix function of \""
             << green <<  A.substr(0, k) << reset
             <<'|' + A.substr(k, A.size()-k)
             <<"\" and \""
             << B.substr(st,i-k+1-st) + '|'
             << green <<  B.substr(i-k+1, k-st)
             << B.substr(0, st)<< reset + "\" = " << k << endl;
#endif
        //если префиксная функция равна длине шаблона, добавляем индекс начала вхождения в результат
        if (k == m)
            return B.size() - i%B.size() - 1;
    }
    return -1;
}

int main() {
    string A;
    string B;
    cin >> A >> B;

    cout << findLoop(A, B);

    return 0;
}