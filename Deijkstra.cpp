#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int N = 1000, INF = 1000000;

class heap // ���������� �������� ���� �� �������. � ����� ������ �������� �������.
{          //��-�� ���� - ������ �� ������� ������ ���� ������ ����
private:   
    int heapSize, arraySize;
    pair<int, int> data[N]; //� ��������� ���� � ������ ���� �� .second ������ ��� ��� ������ ������� �������, � .first - �����, � .second - ���������� �� ��������
    int heapL(int cur) //����� ������� ������ ���� � ������
    {
        return cur * 2 + 1;
    }
    int heapR(int cur)// -//- �������
    {
        return cur * 2 + 2;
    }
    int heapP(int cur) // -//- ��������
    {
        return (cur - 1) / 2;
    }
    void siftUp(int cur); // ����������� �����
    void siftDown(int cur); // ����������� ����
public:
    int insert(pair<int, int> val); // ������� �������� � ����
    pair<int, int> pop(); // ������������ �� ���� ��������
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

void heap::siftUp(int cur) //����������� �����
{
    if(cur == 0) //���� �� � ����� - �����
       return;
    int parent = heapP(cur);
    if(data[cur].second < data[parent].second) // ������������ � ���������, ���� ��-�� ���� ��������
    {                                          // �������� � ���������� �� ��������
        swap(data[cur], data[parent]);         // ��� ����� ������ ������� "��������" ������
        siftUp(parent);
    }
    return;
}

int heap::insert(pair<int, int> val) // ������� � ����
{
    if(heapSize  >= arraySize) // ��������� �� �������� �� �� �� ������� ������������������� �������
        return 1;
    data[heapSize] = val; //������ ����� ������� � ����� � ���������� �����, �������������� ��-�� ����
    siftUp(heapSize);
    heapSize++;
    
    return 0;
}

void heap::siftDown(int cur) // ����������� ����
{
    int ls = heapL(cur), rs = heapR(cur), minIndex; // ��� ���� ������ �������� ����������� ����
    if(ls >= heapSize)                              // ���� �� ������ ����
        return;                                     // ����� ���� ��� ���������� ��������� ��-�� ����
    minIndex = ls;                                  // �� �������� � ��� � ���������� ��� �� ����� ����
    if(rs < heapSize)
        if(data[ls].second > data[rs].second)
            minIndex = rs;
    if(data[cur].second > data[minIndex].second)
    {
        swap(data[cur], data[minIndex]);
        siftDown(minIndex);
    }
}

pair<int, int> heap::pop() //������ ������� �� ���� � ��������� ���

{
    pair<int, int> result = data[0];
    heapSize--;
    data[0] = data[heapSize]; //����� ������ �������� � ��������� � ������ ����
    siftDown(0); //��� ������� ���� � �������, ����� ���� ��������� �� ����
    return result;
}
//����� �������� ����

//�������� �����
vector<vector<pair<int, int> > > g; // ��� ����-���������
bool used[N];                       
int d[N];
heap h = heap();//����

void dijkstra(int a)
{
    h.insert(make_pair(a, d[a]));
    pair<int, int> buf;//����������� buf ���� pair �� ���� ����� �����
    int num, len;
    while(!h.isEmpty())
    {
        buf = h.pop();//�������������
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
    g.resize(n);//������ ������ g �� n
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            cin >> buf;
            if(buf >= 0)
                g[i].push_back(make_pair(j, buf)); // ��� � ������� ������� ��������� �� ������� �����
        }//��� ������ ����� �������� ������ �����, � ������� ����� ������� �� ���� ����� ������ �������� � ���� ��� - ����� ����� � ���  // � ������ ���������, � ��� ����� �� ������ � �������.
    int a;
    cin >> a;
    cout << endl;
    fclose(stdin);
    for(int i = 0; i < N; i++)
        d[i] = INF;     //� ���� ������� � ���� ����� �����, ���� ��������� �� ���� ������ INF
	d[a] = 0;           //� ���������� �� ��������� - ����, ��� �������
	dijkstra(a);        //���������� ��� ��������
						//����� ���������� �� � �� ���� ���������.
    for(int i = 0; i < n; i++)
        cout << d[i] << ' ';
    cout << endl;
    freopen("g.dot", "w",stdout);//��������� ��� ������ ����(������ ���� � ������ ��� ����������). ������� ���������� � �������������� ����.
	cout << "strict graph { " << "\r";//������� dot ����.
	for (int i = 0; i<n; i++){
		 if (a==i) continue;
		cout << a << " -- " << i << "[label=" << d[i] << "]" << "\r";
	}
	cout << "}";
	fclose (stdout);
	system("Graphviz2.38\\bin\\dot.exe g.dot -Tpng -Gcharset=latin1 -og.png");//������� ��������� Graphviz.
	system("g.png");//������� �� ����� ��������
    return 0;
}
