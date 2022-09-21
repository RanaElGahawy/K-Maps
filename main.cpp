#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <string>
using namespace std;

struct Minterm
{
    int decimalnumber;
    string binarynumber;
    bool included;
    int numberof1s;
};

struct Implicants
{
    int group;
    vector<Minterm> includedterms;
    string binarynumber;
};

struct EssentialPrimeImplicants
{
    int minterm;
    vector<string> binaryNumber;
    int NumOfExistence;
};



void INTIALIZE (int &num, int &NumberOfMinterms, vector<Minterm> &minterms);
vector<vector<bool>> DRAW_KMAP (int num, vector<Minterm> &minterms);
void ArrangeinIncreasingOrder (vector<Minterm> Minterms, int NumberOfMinterms);
vector<Implicants> Compare_Minterms (vector<Minterm> Minterms, int NumberOfMinterms, int NumberOfInputs);
vector<EssentialPrimeImplicants> GetEssentialPrimeImplicants (vector<Implicants> PrimeImplicants, vector<Minterm> minterms);
void PrintSimplifiedBooleanExpression (vector<EssentialPrimeImplicants> EPI);

int main()
{
    int num, NumberOfMinterms;
    vector<Minterm> minterms;
    INTIALIZE (num, NumberOfMinterms, minterms);
    vector<vector<bool>> KMap = DRAW_KMAP(num, minterms);

    ArrangeinIncreasingOrder(minterms, NumberOfMinterms);
    cout << endl << endl;
    vector<Implicants> PI = Compare_Minterms(minterms, NumberOfMinterms, num);
    cout << endl;
    vector<EssentialPrimeImplicants> EPI = GetEssentialPrimeImplicants (PI, minterms);
    PrintSimplifiedBooleanExpression(EPI);
    
    return 0;
}



void INTIALIZE (int &num, int &NumberOfMinterms, vector<Minterm> &minterms)
{
    bool valid = true, valid1 = true;
    do
    {
    cout << "Enter the number of inputs:\n";
    cin >> num;
    } while (num != 2 && num != 3);
    cout << "Enter the number of minterms:\n";
    do
    {
        cin >> NumberOfMinterms;
        if (NumberOfMinterms > pow(2,num))
        {
            valid1 =  false;
            cout << "Invalid number of minterms!!\n";
            cout << "Enter the number of minterms again:\n";
        }
    } while (!valid1);
    cout << "Enter the minterms:\n";
    do {
        Minterm minterm;
        for (int i = 0 ; i < NumberOfMinterms; i++)
        {
            cin >> minterm.decimalnumber;
            minterms.push_back(minterm);
            
            if ( minterm.decimalnumber > (pow(2,num)-1))
            {
                valid = false;
                cout << "Invalid minterms!!\n";
                cout << "Enter the minterms again:\n";
                minterms.clear();
            }
        }
    } while (!valid);

}

vector<vector<bool>> DRAW_KMAP (int num, vector<Minterm> &minterms)
{
    vector<vector<bool>> KMap;
    switch (num)
    {
        case 2:
            {
                KMap =
                {
                    {0,0},
                    {0,0}
                };
                for ( int i = 0 ; i < minterms.size(); i++)
                {
                    if (minterms[i].decimalnumber == 0)
                    {KMap[0][0] = 1; minterms[i].binarynumber = "00"; minterms[i].numberof1s = 0; continue;}
                    else if (minterms[i].decimalnumber == 1)
                    {KMap[1][0] = 1; minterms[i].binarynumber = "01"; minterms[i].numberof1s = 1; continue;}
                    else if (minterms[i].decimalnumber == 2)
                    {KMap[0][1] = 1; minterms[i].binarynumber = "10"; minterms[i].numberof1s = 1; continue;}
                    else if (minterms[i].decimalnumber == 3)
                    {KMap[1][1] = 1; minterms[i].binarynumber = "11"; minterms[i].numberof1s = 2;}
                }
                for ( int i = 0 ; i < 2; i++)
                {
                    for ( int j = 0 ; j < 2; j++)
                    {
                        cout << KMap[i][j] << " ";
                    }
                    cout << endl;
                }
                break;
            }
        case 3:
        {
            KMap =
            {
                {0,0,0,0},
                {0,0,0,0}
            };
            for ( int i = 0 ; i < minterms.size(); i++)
            {
                if (minterms[i].decimalnumber == 0)
                {KMap[0][0] = 1; minterms[i].binarynumber = "000"; minterms[i].numberof1s = 0; continue;}
                else if (minterms[i].decimalnumber == 1)
                {KMap[1][0] = 1; minterms[i].binarynumber = "001"; minterms[i].numberof1s = 1; continue;}
                else if (minterms[i].decimalnumber == 2)
                {KMap[0][1] = 1; minterms[i].binarynumber = "010"; minterms[i].numberof1s = 1; continue;}
                else if (minterms[i].decimalnumber == 3)
                {KMap[1][1] = 1; minterms[i].binarynumber = "011"; minterms[i].numberof1s = 2; continue;}
                else if (minterms[i].decimalnumber == 4)
                {KMap[0][3] = 1; minterms[i].binarynumber = "100"; minterms[i].numberof1s = 1; continue;}
                else if (minterms[i].decimalnumber == 5)
                {KMap[1][3] = 1; minterms[i].binarynumber = "101"; minterms[i].numberof1s = 2; continue;}
                else if (minterms[i].decimalnumber == 6)
                {KMap[0][2] = 1; minterms[i].binarynumber = "110"; minterms[i].numberof1s = 2; continue;}
                else if (minterms[i].decimalnumber == 7)
                {
                    KMap[1][2] = 1; minterms[i].binarynumber = "111"; minterms[i].numberof1s = 3;}
            }
            for ( int i = 0 ; i < 2; i++)
            {
                for ( int j = 0 ; j < 4; j++)
                {
                    cout << KMap[i][j] << " ";
                }
                cout << endl;
            }
            break;
        }
    }
    return KMap;
}

void heapify(vector<Minterm> &arr, int n, int i)
{
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l].numberof1s > arr[smallest].numberof1s)
        smallest = l;
  
    if (r < n && arr[r].numberof1s > arr[smallest].numberof1s)
        smallest = r;

    if (smallest != i) {
        swap(arr[i], arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void build_heap(vector<Minterm> &arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}
  
void heap_sort(vector<Minterm> &arr, int n)
{
    build_heap(arr, n);
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void ArrangeinIncreasingOrder (vector<Minterm> Minterms, int NumberOfMinterms)
{
    heap_sort(Minterms, NumberOfMinterms);
    
    for ( int i = 0 ; i < NumberOfMinterms; i++)
    {
        cout << Minterms[i].binarynumber << Minterms[i].numberof1s << endl;
    }
}

vector<Implicants> Compare_Minterms (vector<Minterm> Minterms, int NumberOfMinterms, int NumberOfInputs)
{
    vector<Implicants> semiPrimeImplicants, PrimeImplicants;
    vector<Implicants>::iterator it;
    Implicants test;
    int count;
    int temp;
    for ( int i = 0 ; i < NumberOfMinterms; i++)
    {
        for ( int j = i+1; j < NumberOfMinterms; j++)
        {
            
            count = 0;
            for ( int k = 0 ; k < NumberOfInputs; k++)
            {
                if (Minterms[i].binarynumber[k] != Minterms[j].binarynumber[k])
                {
                    count++;
                    temp = k;
                }
            }
            if (count == 1)
            {
                test.group = i;
                test.includedterms.clear();
                test.includedterms.push_back(Minterms[i]);
                test.includedterms.push_back(Minterms[j]);
                test.binarynumber = Minterms[i].binarynumber;
                test.binarynumber[temp] = 't';
                semiPrimeImplicants.push_back(test);
                
            }
        }
    }
    
    for ( int i = 0 ; i < semiPrimeImplicants.size(); i++)
    {
        cout << "t " << semiPrimeImplicants[i].binarynumber << endl;
    }
    
    cout << endl << endl;
    
    
    int i = 0;
    while (i<semiPrimeImplicants.size())
    {
        for ( int j = 1; j < semiPrimeImplicants.size(); j++)
        {
            count = 0;
            for ( int k = 0 ; k < NumberOfInputs; k++)
            {
                if (semiPrimeImplicants[0].binarynumber[k] != semiPrimeImplicants[j].binarynumber[k])
                {
                    count++;
                    temp = k;
                }
            }
            if (count == 1)
            {
                test.group = i;
                test.includedterms.clear();
                for ( int m = 0 ; m < 2; m++)
                {
                    test.includedterms.push_back(semiPrimeImplicants[i].includedterms[m]);
                    test.includedterms.push_back(semiPrimeImplicants[j].includedterms[m]);
                }
                
                test.binarynumber = semiPrimeImplicants[i].binarynumber;
                test.binarynumber[temp] = 't';
                PrimeImplicants.push_back(test);
                it = semiPrimeImplicants.begin();
                semiPrimeImplicants.erase(it);
            }
        }
        i++;
    }
    
    while ( !semiPrimeImplicants.empty())
    {
        PrimeImplicants.push_back(semiPrimeImplicants[0]);
        it = semiPrimeImplicants.begin();
        semiPrimeImplicants.erase(it);
    }
    
    
    for ( int s = 0 ; s < PrimeImplicants.size(); s++)
    {
        cout << PrimeImplicants[s].binarynumber << endl;
        
    }
    
    cout << endl << endl;

    for ( int i = 0 ; i < PrimeImplicants.size(); i++)
        {
            for ( int j = i+1; j < PrimeImplicants.size(); j++)
            {
                count = 0;
                for ( int k = 0 ; k < NumberOfInputs; k++)
                {
                    if (PrimeImplicants[i].binarynumber[k] != PrimeImplicants[j].binarynumber[k])
                    {
                        count++;
                    }
                }
                if (count == 0)
                {
                    it = PrimeImplicants.begin()+j;
                    PrimeImplicants.erase(it);
                    j--;
                }
            }
        }

    for ( int s = 0 ; s < PrimeImplicants.size(); s++)
    {
        cout << PrimeImplicants[s].binarynumber << endl;
        

    }
    
    return PrimeImplicants;
}

vector<EssentialPrimeImplicants> GetEssentialPrimeImplicants (vector<Implicants> PrimeImplicants, vector<Minterm> minterms)
{
    vector<EssentialPrimeImplicants> PI, EPI;
    EssentialPrimeImplicants test;
 
    for ( int i = 0 ; i < minterms.size(); i++)
    {
        test.minterm = minterms[i].decimalnumber;
        test.NumOfExistence = 0;
        PI.push_back(test);

    }
    
    for ( int i = 0 ; i < PrimeImplicants.size(); i++)
    {
        int count = 0;
        for ( int j = 0 ; j < PrimeImplicants[i].includedterms.size(); j++)
        {
            for ( int k = 0 ; k < minterms.size(); k++)
            {
                if (PI[k].minterm ==  PrimeImplicants[i].includedterms[j].decimalnumber)
                {
                    PI[k].NumOfExistence++;
                    PI[k].binaryNumber.push_back(PrimeImplicants[i].binarynumber);
                }
            }
        }
    }
    cout << endl << endl;
    for ( int i = 0 ; i < PI.size(); i++)
    {
//        count += PI[i]
        if (PI[i].NumOfExistence == 1)
        {
            EPI.push_back(PI[i]);
            cout << PI[i].binaryNumber[0] << endl;
        }
    }
    return EPI;
}

void PrintSimplifiedBooleanExpression (vector<EssentialPrimeImplicants> EPI)
{
    cout << "The Simplified Boolean Expression is: ";
    int j;
    for ( int i = 0 ; i < EPI.size() ; i++)
    {
        if (EPI[i].binaryNumber[0][0] == '1')
            cout << "A";
        else if (EPI[i].binaryNumber[0][0] == '0')
            cout << "A'";
        
        if (EPI[i].binaryNumber[0][1] == '1')
            cout << "B";
        else if (EPI[i].binaryNumber[0][1] == '0')
            cout << "B'";
        
        if (EPI[i].binaryNumber[0].size() == 3 )
        {
            if (EPI[i].binaryNumber[0][2] == '1')
                cout << "C";
            else if (EPI[i].binaryNumber[0][2] == '0')
                cout << "C'";
        }
        j = i+1;
        
        if ( j != EPI.size())
            cout << " + " ;
    }
    
}

