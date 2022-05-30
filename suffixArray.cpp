#include <iostream>

using namespace std;

struct suffixNode
{
    int index, r1, r2;
};

class SuffixArray
{

public:
    suffixNode* suffixNodes;
    int arrSize;
    int* suffixArr;

    SuffixArray (const char* arr)
    {

        arrSize = getSize(arr);
        suffixArr = new int[arrSize];
        suffixNodes = new suffixNode[arrSize];

        int i;
        char character;
        for (i=0; i < arrSize; i++)
        {
            if (arr[0] >= 'a') character = 'a';
            else character = 'A';

            if (arr[i] == '$')
            {
                suffixNodes[i].index = arrSize - 1;
                suffixNodes[i].r1 = 0;
                suffixNodes[i].r2 = -1;
            }
            else if (arr[i+1] == '$')
            {
                suffixNodes[i].index = i;
                suffixNodes[i].r1 = (arr[i] - character) + 1;
                suffixNodes[i].r2 = 0;
            }
            else
            {
                suffixNodes[i].index = i;
                suffixNodes[i].r1 = (arr[i] - character) + 1;
                suffixNodes[i].r2 = (arr[i+1] - character) + 1;
            }

        }

        ConstructUsingPrefixDoubling();

    }

    void ConstructUsingPrefixDoubling()
    {
        for (int k = 2; k < 2*arrSize; k = k*2)
        {
            //Sort the suffix array using <r1,r2>
            mergeSort(suffixNodes, 0, arrSize-1, false);

            int counter = 0;
            int previousCounter = suffixNodes[0].r1;
            suffixNodes[0].r1 = counter;

            //Move sequentially on the array elements to update r1 for each element.
            for (int i = 1; i < arrSize; i++)
            {
                if (suffixNodes[i].r1 == previousCounter && suffixNodes[i].r2 == suffixNodes[i-1].r2)
                {
                    previousCounter = suffixNodes[i].r1;
                    suffixNodes[i].r1 = counter;
                }
                else
                {
                    previousCounter = suffixNodes[i].r1;
                    suffixNodes[i].r1 = ++counter;
                }
            }

            //Sort the suffix array using index.
            mergeSort(suffixNodes, 0, arrSize-1, true); //true means sort by index

            //Move sequentially on the array elements to update r2 for each element.
            for (int i = 0; i < arrSize; i++)
            {
                int nextIndex = suffixNodes[i].index + k;

                if(nextIndex < arrSize)
                {
                    suffixNodes[i].r2 = suffixNodes[nextIndex].r1;
                }
                else
                {
                    suffixNodes[i].r2 = -1;
                }

            }


        }

        //obtain the suffix array by inverse the suffixNodes[i].r1 to index
        for (int i = 0; i < arrSize; i++)
        {
            suffixArr[suffixNodes[i].r1] = i;
        }

    }

    void Print()
    {
        for (int i = 0; i < arrSize; i++)
        {
            cout<< suffixArr[i] <<" ";
        }
          cout<< "\n";
    }

    int getSize(const char* arr)
    {
        int n = 1; //for the $ character

        for(int j=0; arr[j]!= '$'; j++)
            n++;

        this->arrSize = n;
        return n;
    }

    void Merge(suffixNode* arr, int l, int m, int r, bool sortIndex)
    {
        int i, j, k, nl, nr;
        nl=m-l+1;
        nr=r-m;
        int leftR1[nl], rightR1[nr]; //hold r1
        int leftR2[nl], rightR2[nr]; //hold r2
        int leftIndx[nl], rightIndx[nr]; //hold index

        for(i = 0; i<nl; i++)
        {
            leftR1[i] = arr[l+i].r1;
            leftR2[i] = arr[l+i].r2;
            leftIndx[i] = arr[l+i].index;
        }

        for(j = 0; j<nr; j++)
        {
            rightR1[j] = arr[m+1+j].r1;
            rightR2[j] = arr[m+1+j].r2;
            rightIndx[j] = arr[m+1+j].index;
        }


        i=0;
        j=0;
        k=l;
        if(!sortIndex)
        {
            while(i<nl && j<nr)
            {
                if ((leftR1[i] < rightR1[j]) || ((leftR1[i] == rightR1[j]) && (leftR2[i] <= rightR2[j])))
                {
                    arr[k].r1 = leftR1[i];
                    arr[k].r2 = leftR2[i];
                    arr[k].index = leftIndx[i];
                    k++;
                    i++;
                }

                //((leftR1[i] > rightR1[j]) || ((leftR1[i] == rightR1[j]) && (leftR2[i] > rightR2[i])))
                else
                {
                    arr[k].r1 = rightR1[j];
                    arr[k].r2 = rightR2[j];
                    arr[k].index = rightIndx[j];
                    k++;
                    j++;
                }
            }

        }
        else //sort the suffix array by Index
        {
            while(i<nl && j<nr)
            {
                if (leftIndx[i] <= rightIndx[j])
                {
                    arr[k].r1 = leftR1[i];
                    arr[k].r2 = leftR2[i];
                    arr[k].index = leftIndx[i];
                    k++;
                    i++;
                }
                else
                {
                    arr[k].r1 = rightR1[j];
                    arr[k].r2 = rightR2[j];
                    arr[k].index = rightIndx[j];
                    k++;
                    j++;
                }
            }

        }

        while(i<nl)         //extra element in left array
        {

            arr[k].r1 = leftR1[i];
            arr[k].r2 = leftR2[i];
            arr[k].index = leftIndx[i];
            k++;
            i++;
        }
        while(j<nr)       //extra element in right array
        {
            arr[k].r1 = rightR1[j];
            arr[k].r2 = rightR2[j];
            arr[k].index = rightIndx[j];
            k++;
            j++;
        }

    }

    void mergeSort(suffixNode* arr, int l, int r, bool sortIndex)
    {
        if(l<r)
        {
            int m=(l+r)/2;
            mergeSort(arr, l, m, sortIndex);
            mergeSort(arr, m+1, r, sortIndex);
            Merge(arr, l, m, r, sortIndex);
        }
    }

};


int main()
{
    SuffixArray t("ACGACTACGATAAC$");
    t.ConstructUsingPrefixDoubling();

    t.Print(); // Prints:  14 11 12  0  6  3  9 13  1  7  4  2  8 10  5

    return 0;
}
