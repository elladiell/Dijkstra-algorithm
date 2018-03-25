#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int N = 1000, INF = 1000000;

class heap // реализация бинарной кучи на массиве. в корне дерева хранится минимум.
{          //св-во кучи - каждый из сыновей обязан быть больше отца
private:   
    int heapSize, arraySize;
    pair<int, int> data[N]; //я использую пары и создаю кучу по .second потому что так удобно хранить вершины, в .first - номер, в .second - расстояние из дейкстры
    int heapL(int cur) //поиск индекса левого сына в дереве
    {
        return cur * 2 + 1;
    }
    int heapR(int cur)// -//- правого
    {
        return cur * 2 + 2;
    }
    int heapP(int cur) // -//- родителя
    {
        return (cur - 1) / 2;
    }
    void siftUp(int cur); // просеивание вверх
    void siftDown(int cur); // просеивание вниз
public:
    int insert(pair<int, int> val); // вставка элемента в кучу
    pair<int, int> pop(); // вытаскивание из кучи минимума
    bool isEmpty()
    {
        return (heapSize == 0);
    }
    heap()
    {
        heapSize = 0;
        arraySize = N;
    }
};

void heap::siftUp(int cur) //просеивание вверх
{
    if(cur == 0) //если мы в корне - выйти
       return;
    int parent = heapP(cur);
    if(data[cur].second < data[parent].second) // сравниваемся с родителем, если св-во кучи нарушено
    {                                          // меняемся и вызываемся от родителя
        swap(data[cur], data[parent]);         // так самый легкий элемент "всплывет" наверх
        siftUp(parent);
    }
    return;
}

int heap::insert(pair<int, int> val) // вставка в кучу
{
    if(heapSize  >= arraySize) // проверяем не вылезаем ли мы за границы инициализированного массива
        return 1;
    data[heapSize] = val; //пихаем новый элемент в конец и просеиваем вверх, восстанавливая св-во кучи
    siftUp(heapSize);
    heapSize++;
    
    return 0;
}

void heap::siftDown(int cur) // просеивание вниз
{
    int ls = heapL(cur), rs = heapR(cur), minIndex; // тут наша задача выделить наименьшего сына
    if(ls >= heapSize)                              // если он вообще есть
        return;                                     // после чего при нахождении нарушения св-ва кучи
    minIndex = ls;                                  // мы меняемся с ним и вызываемся уже от этого сына
    if(rs < heapSize)
        if(data[ls].second > data[rs].second)
            minIndex = rs;
    if(data[cur].second > data[minIndex].second)
    {
        swap(data[cur], data[minIndex]);
        siftDown(minIndex);
    }
}

pair<int, int> heap::pop() //достаю элемент из кучи и возвращаю его

{
    pair<int, int> result = data[0];
    heapSize--;
    data[0] = data[heapSize]; //после взятия минимума я запихиваю в корень кучи
    siftDown(0); //его крайний лист в массиве, после чего просеиваю ее вниз
    return result;
}
//конец создания кучи

//создание графа
vector<vector<pair<int, int> > > g; // все полу-глобально
bool used[N];                       
int d[N];
heap h = heap();//куча

void dijkstra(int a)
{
    h.insert(make_pair(a, d[a]));
    pair<int, int> buf;//описывается buf типа pair из двух целых чисел
    int num, len;
    while(!h.isEmpty())
    {
        buf = h.pop();//вытаскивается
        num = buf.first;
        len = buf.second;
        for(int i = 0; i < g[num].size(); i++)
        {
            if(!used[g[num][i].first])
            {
                d[g[num][i].first] = min(d[g[num][i].first], d[num] + g[num][i].second);
                h.insert(make_pair(g[num][i].first, d[g[num][i].first]));
            }
        }
        used[num] = true;
    }
}

int main() {
    int n, buf;
    freopen("graphInput.txt", "r", stdin);
    cin >> n;
    g.resize(n);//меняет длинну g на n
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            cin >> buf;
            if(buf >= 0)
                g[i].push_back(make_pair(j, buf)); // тут я загоняю матрицу смежности во входном файле
        }//для каждой точки хранится список точек, в которую можно перейти из этой точки список хранится в виде пар - номер точки и вес  // в список смежности, с ним лучше по памяти и времени.
    int a;
    cin >> a;
    cout << endl;
    fclose(stdin);
    for(int i = 0; i < N; i++)
        d[i] = INF;     //в этом массиве у меня будет ответ, пока растояния до всех вершин INF
	d[a] = 0;           //а расстояние до стартовой - ноль, что логично
	dijkstra(a);        //собственно сам алгоритм
						//вывод расстояний от а до всех остальных.
    for(int i = 0; i < n; i++)
        cout << d[i] << ' ';
    cout << endl;
    freopen("g.dot", "w",stdout);//открываем для записи файл(создаёт файл в случае его отсутствия). Удаляет содержимое и перезаписывает файл.
	cout << "strict graph { " << "\r";//Создаем dot файл.
	for (int i = 0; i<n; i++){
		 if (a==i) continue;
		cout << a << " -- " << i << "[label=" << d[i] << "]" << "\r";
	}
	cout << "}";
	fclose (stdout);
	system("Graphviz2.38\\bin\\dot.exe g.dot -Tpng -Gcharset=latin1 -og.png");//Вызваем программу Graphviz.
	system("g.png");//Выводим на экран картинку
    return 0;
}
