#include <iostream>
using namespace std;

template<class T, int m>
class BTree
{
public:
    struct Node
    {
        T keys[m];
        int numberOfKeys;
        Node* children[m+1];
        Node* parent;
    };

    Node* root;

    BTree()
    {
        root = createNewNode();
    }

    Node* createNewNode ()
    {
        Node* newNode = new Node;

        for(int i=0; i<m; i++)
        {
            newNode->keys[i] = -1;
            newNode->children[i] = NULL;
        }
        newNode->children[m] = NULL; //extra pointer for the last child

        newNode->numberOfKeys = 0;
        newNode->parent = NULL;
        return newNode;
    }

    Node* findLeafNode(Node* node, T key)
    {
        //root node or leaf node
        if(node->children[0] == NULL)
        {
            return node;
        }

        for (int i=0; i < node->numberOfKeys; i++)
        {
            if(key < node->keys[i])
            {
                //recurse -> left
                return findLeafNode(node->children[i], key);
            }
        }

        // if the key is greater than all the keys of the node
        if(key > node->keys[node->numberOfKeys-1])
        {
            //recurse -> right
            return findLeafNode(node->children[node->numberOfKeys], key);
        }

    }

    void split (Node* node)
    {
        Node* tempNode = createNewNode();

        // Root Node
        if(node->parent == NULL)
        {
            root = createNewNode();

            //stores the data of the root node
            root->keys[0] = node->keys[m/2];
            root->numberOfKeys++;
            root->children[0] = node;
            root->children[1] = tempNode;

            node->parent = root;
            tempNode->parent = root;

            //clear the splited key
            node->keys[m/2] = -1;
            node->numberOfKeys--;

            int j=0;
            for(int i=(m/2)+1; i<m; i++) //attach the keys to the new node
            {
                tempNode->keys[j++] = node->keys[i];
                tempNode->numberOfKeys++;

                node->keys[i] = -1;
                node->numberOfKeys--;
            }

            j=0;
            for(int i=(m/2)+1; i<=m; i++) //attach the children's pointers to the new node
            {
                tempNode->children[j++] = node->children[i];
                node->children[i] = NULL;
            }

        }
        else
        {
            //remove the middle key from the node and add it in the parent keys array
            node->parent->keys[node->parent->numberOfKeys] = node->keys[m/2];
            node->parent->numberOfKeys++;
            mergeSort(node->parent->keys, 0, node->parent->numberOfKeys-1);

            //get the index of the splited key after moving it to his parent
            int indx=-1;
            for(int i=0; i < node->parent->numberOfKeys; i++)
            {
                if(node->parent->keys[i] == node->keys[m/2])
                {
                    indx = i;
                }

            }

            //clear the split key
            node->keys[m/2] = -1;
            node->numberOfKeys--;

            int j=0;
            for(int i=(m/2)+1; i<m; i++) //attach the keys to the new node
            {
                tempNode->keys[j++] = node->keys[i];
                tempNode->numberOfKeys++;

                node->keys[i] = -1;
                node->numberOfKeys--;
            }

            tempNode->parent = node->parent;

            Node* newChildren[m+1]; //to hold the new children's pointers
            for(int i=0; i<=m; i++)
            {
                if(i<=indx)
                {
                    //this pointers remain the same after splitting
                    newChildren[i] = node->parent->children[i];

                }
                else if(i == indx+1)
                {
                    //the pointer that points to the new node
                    newChildren[i] = tempNode;
                }
                else
                {
                    //shifting the pointers to the appropriate place
                    newChildren[i] = node->parent->children[i-1];
                }

            }

            //update the parent's array of children
            for(int i=0; i<=m; i++)
            {
                node->parent->children[i] = newChildren[i];
            }

            //check the splitting condition for the parent node
            if (node->parent->numberOfKeys == m )
                split(node->parent);

        }


    }


    void Insert(T key)
    {
        Node* targetNode = findLeafNode(root, key);

        //insert at the last position
        targetNode->keys[targetNode->numberOfKeys] = key;
        targetNode->numberOfKeys++;
        mergeSort(targetNode->keys, 0, targetNode->numberOfKeys-1);

        //if the number of keys equals to max keys
        if(targetNode->numberOfKeys > m-1)
        {
            split(targetNode);
        }

    }


    void print(Node* node, string s)
    {
        if(node == NULL) return;

        for(int i=0; i < node->numberOfKeys-1; i++)
        {
            cout << node->keys[i] <<",";
        }
        cout<< node->keys[node->numberOfKeys-1] << endl;

        s+=" ";
        for(int i=0; i < m; i++)
        {
            if(node->children[i] != NULL)
            {
                cout<<s;
                print(node->children[i], s);
            }

        }
    }

    void Print()
    {
        print(root, "");
    }


    void Merge(T arr[], int l, int f, int r)
    {
        int i, j, k, nl, nr;
        nl=f-l+1;
        nr=r-f;
        int leftA[nl], rightA[nr];
        for(i = 0; i<nl; i++)
            leftA[i] = arr[l+i];
        for(j = 0; j<nr; j++)
            rightA[j] = arr[f+1+j];

        i=0;
        j=0;
        k=l;
        while(i<nl && j<nr)
        {
            if(leftA[i] <= rightA[j]) arr[k++] = leftA[i++];
            else   arr[k++] = rightA[j++];
        }

        while(i<nl)         //extra element in left array
        {
            arr[k++] = leftA[i++];
        }
        while(j<nr)       //extra element in right array
        {
            arr[k++] = rightA[j++];
        }
    }

    void mergeSort(T arr[], int l, int r)
    {
        if(l<r)
        {
            int f=(l+r)/2;
            mergeSort(arr, l, f);
            mergeSort(arr, f+1, r);
            Merge(arr, l, f, r);
        }
    }



    

};

int main()
{
    // Construct a BTree of order 3, which stores int data
    BTree<int,3> t1;

    t1.Insert(1);
    t1.Insert(5);
    t1.Insert(0);
    t1.Insert(4);
    t1.Insert(3);
    t1.Insert(2);

    t1.Print(); // Should output the following on the screen:

    /*
    1,4
      0
      2,3
      5
    */

    // Construct a BTree of order 5, which stores char data
    BTree<char,5> t;

    // Look at the example in our lecture:
    t.Insert('G');
    t.Insert('I');
    t.Insert('B');
    t.Insert('J');
    t.Insert('C');
    t.Insert('A');
    t.Insert('K');
    t.Insert('E');
    t.Insert('D');
    t.Insert('S');
    t.Insert('T');
    t.Insert('R');
    t.Insert('L');
    t.Insert('F');
    t.Insert('H');
    t.Insert('M');
    t.Insert('N');
    t.Insert('P');
    t.Insert('Q');

    t.Print(); // Should output the following on the screen:

    /*
    K
      C,G
        A,B
        D,E,F
        H,I,J
      N,R
        L,M
        P,Q
        S,T
    */

    return 0;
}

