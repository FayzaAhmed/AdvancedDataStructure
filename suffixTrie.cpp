#include <iostream>
using namespace std;

struct suffixNode;
template <class T>
struct Node       //linked list node
{
    T data;
    Node<T>* next;
    suffixNode* child;
    Node()
    {
        next = NULL;
        child = NULL;
    }
};

template <class T>
class LinkedList
{
public:

    Node<T>* head;

    LinkedList()
    {
        head = NULL;
    }

    LinkedList(T data)
    {
        this->data = data;
    }

    void add(T new_data)
    {
        Node<T>* new_node = new Node<T>;
        new_node->data = new_data;
        new_node->next = NULL;

        if (head == NULL)
        {
            head = new_node;
            return;
        }
        else
        {
            Node<T>* temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = new_node;
            return;
        }
    }

    Node<T>* Search(T data)
    {
        Node<T>* current = head;
        while(current!=nullptr)
        {
            if(current->data == data)
            {
                return current;
            }
            current=current->next;
        }
        return nullptr;
    }

    void display()
    {
        Node<T>* node = new Node<T>;
        node = head;
        while (node != NULL)
        {
            cout << node->data <<" ";
            node = node->next;
        }
        cout <<"\n";
    }


    int Size()
    {
        int counter = 0;
        Node<T>* temp = new Node<T>;
        temp = head;
        while (temp != NULL)
        {
            ++counter;
            temp = temp->next;
        }
        return counter;
    }

    //destructor
    ~LinkedList()
    {
        Node<T>* ptr = head;
        while (ptr != NULL)
        {
            head = head->next;
            delete ptr;
            ptr = head;
        }
    }
};

struct suffixNode
{
    LinkedList<char> children;
    int index;
    suffixNode()
    {
        index = -1;
    }
};

class SuffixTrie
{
public:
    suffixNode* root = NULL;

    SuffixTrie(char* arr)    //parametrized constructor
    {
        char* temp = arr;
        int suffixSize = getSize(arr);
        int arrSize = suffixSize;

        insertSuffix(arr, arrSize, suffixSize);
        for(int i = 1; i < arrSize; i++)       //get all suffixes
        {
            temp = updateSuffix(temp, suffixSize);
            suffixSize--;
            insertSuffix(temp, arrSize, suffixSize);
        }
    }

    char* updateSuffix(char* arr, int n)   //trimming the first char each time
    {
        char* temp = new char[n];
        for(int i = 0; i < n-1; i++)
        {
            temp[i] = arr[i+1];
        }

        return temp;
    }

    int getSize(char* arr)
    {
        int j = 0;
        int n = 0;
        while(*(arr+j) != NULL)
        {
            n++;
            j++;
        }
        return n;
    }

    void insertSuffix(char* suffix, int arrSize, int suffixSize)
    {
        if(root == NULL)
        {
            root = new suffixNode;
        }
        suffixNode* alternativeRoot = root;
        int idx = 0;

        //the character already exist in the suffix node
        while(alternativeRoot->children.Search(suffix[idx]) != NULL)
        {
            Node<char>* node = alternativeRoot->children.Search(suffix[idx]);
            alternativeRoot = node->child;
            idx++;
        }
        //add the new character
        alternativeRoot->children.add(suffix[idx]);

        //store the linked list node of the new character in temp
        Node<char>* temp = alternativeRoot->children.Search(suffix[idx]);

        for(int i = idx + 1; i < suffixSize; i++)
        {
            suffixNode* newNode = new suffixNode;
            temp->child = newNode;
            alternativeRoot = newNode;
            alternativeRoot->children.add(suffix[i]);
            temp = alternativeRoot->children.Search(suffix[i]);
        }

        suffixNode* lastNode = new suffixNode;
        temp->child = lastNode;
        alternativeRoot = lastNode;
        alternativeRoot->index = arrSize - suffixSize;
    }

    void getAllIndexes(suffixNode* curNode)
    {
        int n = curNode->children.Size();
        if(curNode->children.Size() == 0)
        {
            cout << curNode->index << " ";
            return;
        }

        Node<char>* newNode = curNode->children.head;

        for(int i = 0; i < n; i++)
        {
            getAllIndexes(newNode->child);
            newNode = newNode->next;
        }
    }

    void Search(char* subArr)
    {
        int n = getSize(subArr);
        suffixNode* Root = root;
        for(int i = 0; i < n; i++)
        {
            if(Root->children.Search(subArr[i]) == NULL)
                return;

            Root = Root->children.Search(subArr[i])->child;
        }
        getAllIndexes(Root);
        cout<<"\n";
    }
};


int main()
{
    // Construct a suffix trie containing all suffixes of "bananabanaba$"

    SuffixTrie t("bananabanaba$");
    t.Search("ana"); // Prints: 1 3 7
    t.Search("naba"); // Prints: 4 8

    return 0;
}
