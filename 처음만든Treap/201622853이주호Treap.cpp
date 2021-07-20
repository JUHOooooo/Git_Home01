#include <iostream>
#include <algorithm>
using namespace std;

int level=1;
int level2 = 0;
class corona_nation
{
public:
    corona_nation(string name = "empty", int num_of_confirmed_p = 0, int num_of_death_p = 0);
    void print_info();

    string m_name;
    int m_num_of_confirmed_p;
    int m_num_of_death_p;
    double m_death_rate = 0;
};

template<class T>
class DLLNode
{
public:
    DLLNode()
    {
        left = right = prev = NULL;
    }
    DLLNode(const T& el, DLLNode* l = NULL, DLLNode* p = NULL, DLLNode* r = NULL) {
        info = el;
        left = l;
        right = r;
        prev = p;
    }
    T info;
    DLLNode* left, * right, * prev;
};

template<class T>
class DLL
{
public:
    DLL()
    {
        head = NULL; // null로 지정하지 않으면?
    }

    ~DLL()
    {
        for (DLLNode<T>* p = head; p != NULL; )
        {
            if (head->left)
            {
                p = head->left;
                delete head;
                head = p;
            }
            if (head->right)
            {
                p = head->right;
                delete head;
                head = p;
            }
        }
    }
    void AddtoDLLSorted(const T& el);  //구현할것
    void topupperleft(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q);
    void topupperright(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q);
    void upperleft(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q);
    void upperright(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q);
    void addleft(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode);
    void addright(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode);
    void deleteDLLSorted(const string& name); // 구현할것
    void searchname(string& name);
    void findname(DLLNode<T>* p, string& name);
    void printtDLLSorted();
    void print(DLLNode<T>* p); //구현할것
private:
    DLLNode<T>* head;
};
corona_nation::corona_nation(string name, int num_of_confirmed_p, int num_of_death_p)
{
    m_name = name;
    m_num_of_confirmed_p = num_of_confirmed_p;
    m_num_of_death_p = num_of_death_p;
    m_death_rate = ((double)m_num_of_death_p / m_num_of_confirmed_p) * 100;
}

void corona_nation::print_info()
{
    cout << m_name;
    cout.width(10);
    cout << m_num_of_confirmed_p;
    cout.width(10);
    cout << m_num_of_death_p;
    cout.width(10);
    cout << m_death_rate;
    cout.width(10);
    cout << level2 << endl;

}
/////////////////////////////////////////////////////////////////////////////////////여기부터 제가 구현한 함수들입니다.
template<class T>
void DLL<T>::AddtoDLLSorted(const T& el)
{
    DLLNode<corona_nation>* p = head;
    DLLNode<corona_nation>* temp;
    DLLNode<corona_nation>* Newnode = new DLLNode<corona_nation>;
    Newnode->info = el;
    //head가 빈 경우
    if (!head) {
        head = Newnode;
    }
    //head가 비어있지 않은 경우
    else
    {
        if (p->info.m_death_rate > Newnode->info.m_death_rate)
        {
            if (p->info.m_name.compare(Newnode->info.m_name) > 0)
            {
                addleft(p, Newnode);
            }
            else if (p->info.m_name.compare(Newnode->info.m_name) < 0)
            {
                addright(p, Newnode);
            }
        }
        else if (p->info.m_death_rate < Newnode->info.m_death_rate)
        {
            if (p->info.m_name.compare(Newnode->info.m_name) > 0)
            {
                topupperleft(p, Newnode, 0);//처음 탐색 부분이기에 root를 기준으로 추가
            }
            else if (p->info.m_name.compare(Newnode->info.m_name) < 0)
            {
                topupperright(p, Newnode, 1);//처음 탐색 부분이기에 root를 기준으로 추가
            }
        }
    }

}
//root의 왼쪽 위에 노드 추가될때
template<class T>
void DLL<T>::topupperleft(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q) {
    head = Newnode;
    head->right = p;
    p->prev = head;
    if (p->left != NULL)
    {
        head->left = p->left;
        p->left->prev = head;
        p->left = NULL;
    }
}

//root의 오른쪽 위에 노드 추가될때
template<class T>
void DLL<T>::topupperright(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q) {
    head = Newnode;
    head->left = p;
    p->prev = head;
    if (p->right != NULL)
    {
        head->right = p->right;
        p->right->prev = head;
        p->right = NULL;
    }
}

//왼쪽 위 노드추가, 전 노드에서 왼쪽에서 가리키는지, 오른쪽에서 가리키는지 구분한다.
template<class T>
void DLL<T>::upperleft(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q) {
	if (q == 0)
	{
        if (p->prev!=NULL)
        {
            p->prev->left = Newnode;
            Newnode = p->prev->left;
            Newnode->right = p;
            if (p->left != NULL)
            {
                Newnode->left = p->left;
                p->left->prev = Newnode;
                p->left = NULL;
            }
        }
        if (p->prev==NULL)
        {
            head->left = Newnode;
            Newnode = head->left;
            Newnode->right = p;
            if (p->left != NULL)
            {
                Newnode->left = p->left;
                p->left->prev = Newnode;
                p->left = NULL;
            }
        }
	}
	else if (q == 1) {
        if (p->prev!=NULL)
        {
            p->prev->right = Newnode;
            Newnode = p->prev->right;
            Newnode->right = p;
            if (p->left != NULL)
            {
                Newnode->left = p->left;
                p->left->prev = Newnode;
                p->left = NULL;
            }
        }
        if (p->prev == NULL)
        {
            head->right = Newnode;
            Newnode = head->right;
            Newnode->right = p;
            if (p->left != NULL)
            {
                Newnode->left = p->left;
                p->left->prev = Newnode;
                p->left = NULL;
            }
        }
	}
}

//오른쪽 위 노드추가, 전 노드에서 왼쪽에서 가리키는지, 오른쪽에서 가리키는지 구분한다.
template<class T>
void DLL<T>::upperright(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode, int q) {
	if (q == 0)
	{
        if (p->prev!=NULL)
        {
            p->prev->left = Newnode;
            Newnode = p->prev->left;
            Newnode->left = p;
            if (p->right != NULL)
            {
                Newnode->right = p->right;
                p->right->prev = Newnode;
                p->right = NULL;
            }
        }
        if (p->prev == NULL)
        {
            head->left = Newnode;
            Newnode = head->left;
            Newnode->left = p;
            if (p->right != NULL)
            {
                Newnode->right = p->right;
                p->right->prev = Newnode;
                p->right = NULL;
            }
        }
	}
	else if (q == 1) {
        if (p->prev!=NULL)
        {
            p->prev->right = Newnode;
            Newnode = p->prev->right;
            Newnode->left = p;
            if (p->right != NULL)
            {
                Newnode->right = p->right;
                p->right->prev = Newnode;
                p->right = NULL;
            }
        }
        if (p->prev == NULL)
        {
            head->right = Newnode;
            Newnode = head->right;
            Newnode->left = p;
            if (p->right != NULL)
            {
                Newnode->right = p->right;
                p->right->prev = Newnode;
                p->right = NULL;
            }
        }
	}
}

//왼쪽 아래 탐색, 추가
template<class T>
void DLL<T>::addleft(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode) {
    if (p->left == NULL)
    {
        p->left = Newnode;
        Newnode->prev = p;
    }
    else
    {
        p = p->left;
        if (p->info.m_death_rate > Newnode->info.m_death_rate)
        {
            if (p->info.m_name.compare(Newnode->info.m_name) > 0)
            {
                addleft(p, Newnode);
            }
            else if (p->info.m_name.compare(Newnode->info.m_name) < 0)
            {
                addright(p, Newnode);
            }
        }
        else if (p->info.m_death_rate < Newnode->info.m_death_rate)
        {
            if (p->info.m_name.compare(Newnode->info.m_name) > 0)
            {
                upperleft(p, Newnode, 0);
            }
            else if (p->info.m_name.compare(Newnode->info.m_name) < 0)
            {
                upperright(p, Newnode, 0);
            }
        }
    }
}
//오른쪽 아래 탐색,추가
template<class T>
void DLL<T>::addright(DLLNode<corona_nation>* p, DLLNode<corona_nation>* Newnode) {
    if (p->right == NULL)
    {
        p->right = Newnode;
        Newnode->prev = p;
    }
    else
    {
        p = p->right;
        if (p->info.m_death_rate > Newnode->info.m_death_rate)
        {
            if (p->info.m_name.compare(Newnode->info.m_name) > 0)
            {
                addleft(p, Newnode);
            }
            else if (p->info.m_name.compare(Newnode->info.m_name) < 0)
            {
                addright(p, Newnode);
            }
        }
        else if (p->info.m_death_rate < Newnode->info.m_death_rate)
        {
            if (p->info.m_name.compare(Newnode->info.m_name) > 0)
            {
                upperleft(p, Newnode, 1);
            }
            else if (p->info.m_name.compare(Newnode->info.m_name) < 0)
            {
                upperright(p, Newnode, 1);
            }
        }
    }
}

// 니즈 탐색
template<class T>
void DLL<T>::searchname(string& name) {
    DLLNode<corona_nation>* p = head;
    p->info.print_info();
    if (p->info.m_name.compare(name)!=0)
    {
        if (p->info.m_name.compare(name)<0)
        {
            p = p->right;
            findname(p,name);
        }
        else if (p->info.m_name.compare(name) > 0)
        {
            p = p->left;
            findname(p, name);
        }
    }
}
template<class T>
void DLL<T>::findname(DLLNode<T>* p,string& name) {
    level2++;
    p->info.print_info();
    if (p->info.m_name.compare(name) != 0)
    {
        if (p->right != NULL && p->info.m_name.compare(name) < 0)
        {
            p = p->right;
            findname(p, name);
        }
        else if (p->left != NULL && p->info.m_name.compare(name) > 0)
        {
            p = p->left;
            findname(p, name);
        }
    }
}

//전체 출력
template<class T>
void DLL<T>::printtDLLSorted()
{
    DLLNode<corona_nation>* p = head;
    p->info.print_info();
    print(p);
}
template<class T>
void DLL<T>::print(DLLNode<T>* p)
{
    int a = 0;
    DLLNode<corona_nation>* pl = p;
    if (pl->left != NULL || pl->right != NULL)
    {
        a = level2;
        level2++;
        if (pl->left!=NULL)
        {
            pl->left->info.print_info();
            print(pl->left);
        }
        level2 = a;
        level2++;
        if (pl->right!=NULL)
        {
            pl->right->info.print_info();
            print(pl->right);
        }
        
    }
}

int main()
{
    DLL<corona_nation> DLLdata;
    cout << "이름";
    cout.width(10);
    cout << "확진자";
    cout.width(10);
    cout << "사망자";
    cout.width(10);
    cout << "사망률";
    cout.width(10);
    cout << "Level" << endl;
    DLLdata.AddtoDLLSorted(corona_nation("D", 100, 60));
    DLLdata.AddtoDLLSorted(corona_nation("F", 100, 75));
    DLLdata.AddtoDLLSorted(corona_nation("H", 100, 14));
    DLLdata.AddtoDLLSorted(corona_nation("C", 100, 70));
    DLLdata.AddtoDLLSorted(corona_nation("A", 100, 55));
    DLLdata.AddtoDLLSorted(corona_nation("I", 100, 65));
    DLLdata.AddtoDLLSorted(corona_nation("J", 100, 45));
    DLLdata.printtDLLSorted();
    string find;
    cout <<"찾을 국가:" <<endl;
    cin >> find;
    level2 = 0;
    DLLdata.searchname(find);


    return 0;
}