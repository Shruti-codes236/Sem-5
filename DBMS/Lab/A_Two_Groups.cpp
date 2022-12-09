#include <bits/stdc++.h>
typedef long long lol;
#define rep(i, a, b) for (lol i = a; i < b; i++)
#define xrep(i, a, b) for (lol i = a; i >= b; i--)
#define pb(x) push_back(x)
#define all(v) v.begin, v.end()
#define min3(a, b, c) min(c, min(a, b))
#define min4(a, b, c, d) min(d, min(c, min(a, b)))
#define sz(x) int((x).size())

const int MOD = 1e9 + 7;
const int modm = 1e5;
void minimize(int &a, int b) { a = std::min(a, b); }
void maximize(int &a, int b) { a = std::max(a, b); }
using namespace std;

vector<lol> odd, even;

// vector<lol> v;
//  using vi = vector<int>;
//  using vvi = vector<vi>;
//  using vll = vector<lol>;
//  using vvll = vector<vll>;
//  using vb = vector<bool>;

// namespace __input
// {
//     template <class T1, class T2>
//     void re(pair<T1, T2> &p);
//     template <class T>
//     void re(vector<T> &a);
//     template <class T, size_t SZ>
//     void re(array<T, SZ> &a);

//     template <class T>
//     void re(T &x) { cin >> x; }
//     void re(double &x)
//     {
//         string t;
//         re(t);
//         x = stod(t);
//     }
//     template <class Arg, class... Args>
//     void re(Arg &first, Args &...rest)
//     {
//         re(first);
//         re(rest...);
//     }

//     template <class T1, class T2>
//     void re(pair<T1, T2> &p) { re(p.f, p.s); }
//     template <class T>
//     void re(vector<T> &a)
//     {
//         for (int i = 0; i < sz(a); i++)
//             re(a[i]);
//     }
//     template <class T, size_t SZ>
//     void re(array<T, SZ> &a)
//     {
//         for (int i = 0; i < SZ; i++)
//             re(a[i]);
//     }
// }
// using namespace __input;

// // string s = "";

// // typedef pair <int, int> pii;

// // int swap(long long int *a, long long int *b)
// // {
// //     long long int temp;
// //     temp = *a;
// //     *a = *b;
// //     *b = temp;
// //     return 0;
// // }

// bool IsPowerOfTwo(long long int x)
// {
//     return (x != 0) && ((x & (x - 1)) == 0);
// }
// lol power(lol a, lol b)
// {
//     lol x = a;
//     while (--b)
//     {
//         x = ((x % MOD) * (a % MOD)) % MOD;
//     }
//     return x;
// }

// void printDivisors(lol n)
// {
//     vector<int> v;
//     // Note that this loop runs till square root
//     for (lol i = 1; i <= sqrt(n); i++)
//     {
//         if (n % i == 0)
//         {
//             // If divisors are equal, print only one
//             if (n / i == i)
//             {
//                 v.pb(i);
//             }

//             else // Otherwise print both
//             {
//                 v.pb(i);
//                 v.pb(n / i);
//             }
//         }
//     }
// }
// bool isPalindrome(string S)
// {
//     // Stores the reverse of the
//     // string S
//     string P = S;

//     // Reverse the string P
//     reverse(P.begin(), P.end());

//     // If S is equal to P
//     if (S == P)
//     {
//         // Return "Yes"
//         return 1;
//     }
//     // Otherwise
//     else
//     {
//         // return "No"
//         return 0;
//     }
// }
// lol sum(vector<lol> a)
// {
//     lol sum_v = 0, i;
//     rep(i, 0, a.size()) { sum_v += a[i]; }
//     return sum_v;
// }
// vector <int> v(MOD,-1);

void shru(lol tc)
{
    lol n,i,x,s1=0,s2=0;
    cin>>n;
    lol a[n];
    rep(i,0,n)
    {
        cin>>a[i];
    }
    sort(a,a+n);
    rep(i,0,n)
    {
        if(a[i]>=0)
        {
            x=i;
            break;
        }
        s1+=a[i];
    }
    rep(i,x,n)
    {
        s2+=a[i];
    }
    cout<<max(abs(s1)-abs(s2),abs(s2)-abs(s1))<<'\n';
    // lol n,i,sum1=0,sum2=0;
    // cin>>n;
    // rep(i,0,n){if(i%2==0){odd.pb(i+1);sum1+=(i+1);}else{even.pb(i+1);sum2+=(i+1);}}
    // lol ind1=odd.size(),ind2=even.size(),x,y;
    // x=ind1;y=ind2;
    // rep(i, 0, x)
    // {
    //     cout << odd[i] << " ";
    // }
    // cout << '\n';
    // rep(i, 0, y)
    // {
    //     cout << even[i] << " ";
    // }
    // cout << '\n';
    // while(sum1!=sum2 and ind1>=0 and ind2>=0)
    // {
    //     swap(odd[ind1],even[ind2]);
    //     sum1=sum(odd);sum2=sum(even);
    //     cout<<sum1<<" "<<sum2<<'\n';
    //     cout << ind1 << " " << ind2 << '\n';
    //     ind1--;ind2--;
    // }
    // if(sum1!=sum2)
    // {
    //     cout<<"NO"<<'\n';
    // }
    // else{
    //     cout<<"YES"<<'\n';
    //     rep(i,0,x)
    //     {
    //         cout<<odd[i]<<" ";
    //     }
    //     cout<<'\n';
    //     rep(i,0,y)
    //     {
    //         cout << even[i] << " ";
    //     }
    //     cout << '\n';
    // }
    // int t;
    // cin>>t;
    // while(t--)
    // {
    //     int n,i;
    //     int d, u;
    //     vector<string> v1, v2;
    //     vector<int> d1, d2, u1, u2;
    //     map <string,int> mp1,mp2;
    //     rep(i,0,n)
    //     {
    //     string c;
    //     cin>>c>>d>>u;
    //     v1.push_back(c);
    //     //v2.push_back(c);
    //     mp1[c]=d;
    //     d1.push_back(d);
    //     }
    //     sort(v1.begin(),v1.end());
    //     sort(d1.begin(),d1.end());
    //     rep(i,0,n)
    //     {
    //         mp2[v1[i]] = d1[i];
    //     }
    //     rep(i,0,n)
    //     {

    //     }
    // }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    lol TC = 1, i;
    cin >> TC;
    //       calculate();
    rep(i, 1, TC + 1)
    {
        // cout<<"Case #"<<TC<<" ";
        shru(i);
    }
    return 0;
}