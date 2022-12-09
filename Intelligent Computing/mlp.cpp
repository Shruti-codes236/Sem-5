#include <bits/stdc++.h>
using namespace std;

double trainset[120][4];
double testset[30][4];
vector <string> data;
vector<double> hiddenLayer;
vector <double>cost,V,Y,err,M;
vector<vector<double> > w1, w2;
double weights[4][3];
#define M 4
#define N 3
#define learning_rate 0.5 

double activation_func(double x)
{
    return 1.0/(1.0+exp(-x));
}

void phi_neuron(int l)
{
    for(int i=0;i<3;i++)
    {
        int sum=0;
        for(int j=0;j<4;j++)
        {
            sum+=weights[j][i]*trainset[l][j];
        }
        V[i]=sum;
    }
    
}

void shuffle()
{
    srand(time(NULL));
    for (int i = 150 - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(data[i], data[j]);
    }
}

double feed_forward(void)
{
    for(int i=0;i<150;i++)
    {
        phi_neuron(i);
        for(int j=0;j<N;j++)
        {
            Y[j]=activation_func(V[j]);
        }
    }

    for (int i=0;i<M;j++)
    {
        V[i]=0;
        for(int j=0;j<N;j++)
        {
            V[i]+=weights[j][i]*trainset[j][i];
        }
        Y[i]=activation_func(V[i]);
    }

    double mx = -1;
    int res = 0;
    for (int i = 0; i < 3; i++)
    {
        if (y[i] > mx)
        {
            mx = Y[i];
            res = i;
        }
    }
    return mx;
}

void ErrorFunc()
{
    for (int i = 0; i < M; i++)
    {
        err[i] = O[i] - Y[i];
    }
}

double inst_cost_func(int e)
{
    int cost=0;
    for(int i=0;i<N;i++)
    {
        cost+=weights(e);
    }
    return cost/2;
}

void back_prop()
{
    vector<double> delta1(N), delta2(M);
    for (int i = 0; i < N; i++)
    {
        delta1[i] = err[i] * Y[i] * (1 - Y[i]);
    }

    for (int j = 0; j < M; j++)
    {
        double sum = 0;
        for (int k = 0; k < M; k++)
        {
            sum += delta1[k] * weights[k][j];
        }
        delta2[j] = V[j] * (1 - V[j]) * sum;
    }
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            w2[i][j] += learning_rate * delta1[i] * Y[j];
        }
    }
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            w1[i][j] += learning_rate * delta2[i] * V[j];
        }
    }
}




int main()
{
    fstream newfile;
    newfile.open("iris_data.txt"); 
    if (newfile.is_open())
    {
        string tp;
        int x = 0, y = 0;
        while (getline(newfile, tp))
        {
            data.push_back(tp);
            int n = tp.size(), i = 0;
            int cnt = 0;
            y=0;
            while (i < n and cnt <= 3)
            {
                string s = "";
                while (tp[i] != ',' && tp[i] != '\n')
                {
                    s += tp[i];
                    i++;
                }
                cnt++;
                i++;
                double num = stof(s);
                trainset[x][y] = num;
                
                cout << trainset[x][y] << " ";
                y++;
            }
            cout<<'\n';
            x++;
        }
        newfile.close(); 
    }
    for (int i = 0; i < 150; i++)
    {
        cout<<i+1<<" ";
        for (int j = 0; j < 4; j++)
        {
            cout << trainset[i][j] << " ";
        }
        cout << '\n';
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<3;j++)
        {
            int x=rand();
            if(x%3==0)
            { weights[i][j]=0;}
            else if(x%3==1)
            {weights[i][j]=1;}
            else
            {weights[i][j]=-1;}
        }
    }
    for (int e = 0; e < 1000; e++)
    {
        for (int i = 0; i < 150; i++)
        {
            feed_forward();
            ErrorFunc();
            back_prop();
        }
    }

    double accuracy = 0;

    for (int i = 0; i < 150; i++)
    {
        accuracy += V[i];
    }

    accuracy = accuracy * 2 / 3;

    cout << accuracy << endl;

    return 0;
}



