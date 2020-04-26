#include <iostream>
#include <string>
#include <vector>
//#define DBG
using namespace std;

//функция подсчета префиксной функции для строки
vector<int> prefix(string str){
    //создаем вектор необходимого размера
    vector<int> p(str.length()+1, 0);
    p[0] = 0;
    for (int i = 1; i <= str.length(); i++) {
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

vector<int> findSubstring(string temp, string text){
    //считаем префиксную функцию для шаблона
    vector<int> p = prefix(temp);
#ifdef DBG
    cout << "Prefix function of " << temp << " : ";
    for (int i : p)
        cout << i << ' ';
    cout << endl;
#endif
    vector<int> res;
    int m = temp.length();
    int k = 0;
    //начинаем проверять равенство символов начиная с нулевого символа
    for (int i = 0; i < text.length(); i++) {

        while (k > 0 && temp[k] != text[i])
            k = p[k - 1];
        //если мы вышли по условию совпадения символов увеличим k, но в вектор уже ничего не добавляем
        if (temp[k] == text[i])
            k++;
#ifdef DBG
        const std::string green("\033[0;32m");
        const std::string reset("\033[0m");

        cout << "Prefix function of \""
        << green << temp.substr(0, k) << reset
        << '|' + temp.substr(k, temp.length()-k) <<
        "\" and \"" + text.substr(0, i-k+1) + '|'
        << green << text.substr(i-k+1, k) << reset
        << "\" = " << k << endl;
#endif
        //если префиксная функция равна длине шаблона, добавляем индекс начала вхождения в результат
        if (k == m)
            res.push_back(i - m + 1);
    }
    return res;
}

int main() {
    string temp;
    string text;
    cin >> temp;
    cin >> text;

    vector<int> res = findSubstring(temp, text);

    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << ((i == res.size() - 1) ? "" : ",");
    }
    if (res.empty())
        cout << -1;
    return 0;
}