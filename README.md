# Eksperimentiniai MyVector klasės tyrimai

##  Klasės naudojimas ir veikimo principas
```MyVector``` tai yra klasė kurį yra alternatyva ```std::vector``` konteineriui turinti pilna jo funkcionalumą.
###
Norint pasinaudoti ```MyVector``` konteineriu įkelkite ```MyVector.h``` į jūsų projektą.

## 1. Funkcijų veikimo pavyzdžiai

### erase()

Testavimo kodas:

```c++
#include <vector>
#include <iostream>
 
 
void print_container(const MyVector<int>& c) 
{
    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';
}
 
int main( )
{
    MyVector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_container(c);
 
    c.erase(c.begin());
    print_container(c);
 
    c.erase(c.begin()+2, c.begin()+5);
    print_container(c);
 
    // Erase all even numbers (C++11 and later)
    for (auto it = c.begin(); it != c.end(); ) {
        if (*it % 2 == 0) {
            it = c.erase(it);
        } else {
            ++it;
        }
    }
    print_container(c);
}
```

MyVector rezultatai:
```
0 1 2 3 4 5 6 7 8 9
1 2 3 4 5 6 7 8 9
1 2 6 7 8 9
1 7 9
```

std::vector rezultatai:
```
0 1 2 3 4 5 6 7 8 9
1 2 3 4 5 6 7 8 9
1 2 6 7 8 9
1 7 9
```

### empty()

Testavimo kodas:

```c++
#include <vector>
#include <iostream>
 
int main()
{
    std::cout << std::boolalpha;
    MyVector<int> numbers;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';
 
    numbers.push_back(42);
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';

```

MyVector rezultatai:
```
Initially, numbers.empty(): true
After adding elements, numbers.empty(): false
```

std::vector rezultatai:
```
Initially, numbers.empty(): true
After adding elements, numbers.empty(): false
```

### insert()

Testavimo kodas:

```c++
#include <iostream>
#include <vector>
 
void print_vec(const MyVector<int>& vec)
{
    for (auto x: vec) {
         std::cout << ' ' << x;
    }
    std::cout << '\n';
}
 
int main ()
{
    MyVector<int> vec(3,100);
    print_vec(vec);
 
    auto it = vec.begin();
    it = vec.insert(it, 200);
    print_vec(vec);
 
    vec.insert(it,2,300);
    print_vec(vec);
 
    // "it" no longer valid, get a new one:
    it = vec.begin();
 
    MyVector<int> vec2(2,400);
    vec.insert(it+2, vec2.begin(), vec2.end());
    print_vec(vec);
 
    int arr[] = { 501,502,503 };
    vec.insert(vec.begin(), arr, arr+3);
    print_vec(vec);
}

```

MyVector rezultatai:
```
 100 100 100
 200 100 100 100
 300 300 200 100 100 100
 300 300 400 400 200 100 100 100
 501 502 503 300 300 400 400 200 100 100 100
```

std::vector rezultatai:
```
100 100 100
200 100 100 100
300 300 200 100 100 100
300 300 400 400 200 100 100 100
501 502 503 300 300 400 400 200 100 100 100
```

### operatoriai( == != < <= > >= )

Testavimo kodas:

```c++
#include <iostream>
#include <vector>
 
int main()
{
    MyVector<int> alice{1, 2, 3};
    MyVector<int> bob{7, 8, 9, 10};
    MyVector<int> eve{1, 2, 3};
 
    std::cout << std::boolalpha;
 
    // Compare non equal containers
    std::cout << "alice == bob returns " << (alice == bob) << '\n';
    std::cout << "alice != bob returns " << (alice != bob) << '\n';
    std::cout << "alice <  bob returns " << (alice < bob) << '\n';
    std::cout << "alice <= bob returns " << (alice <= bob) << '\n';
    std::cout << "alice >  bob returns " << (alice > bob) << '\n';
    std::cout << "alice >= bob returns " << (alice >= bob) << '\n';
 
    std::cout << '\n';
 
    // Compare equal containers
    std::cout << "alice == eve returns " << (alice == eve) << '\n';
    std::cout << "alice != eve returns " << (alice != eve) << '\n';
    std::cout << "alice <  eve returns " << (alice < eve) << '\n';
    std::cout << "alice <= eve returns " << (alice <= eve) << '\n';
    std::cout << "alice >  eve returns " << (alice > eve) << '\n';
    std::cout << "alice >= eve returns " << (alice >= eve) << '\n';
}
```

MyVector rezultatai:
```
alice == bob returns false
alice != bob returns true
alice <  bob returns true
alice <= bob returns true
alice >  bob returns false
alice >= bob returns false

alice == eve returns true
alice != eve returns false
alice <  eve returns false
alice <= eve returns true
alice >  eve returns false
alice >= eve returns true

```

std::vector rezultatai:
```
alice == bob returns false
alice != bob returns true
alice <  bob returns true
alice <= bob returns true
alice >  bob returns false
alice >= bob returns false

alice == eve returns true
alice != eve returns false
alice <  eve returns false
alice <= eve returns true
alice >  eve returns false
alice >= eve returns true

```


## 2. Efektyvumo/spartos analizę ( push_back(int) )

| Elementų skaičius |   Laikas naudojant std::vector    |   Laikas naudojant MyVector      |
| ----------------  | --------------------------------- | -------------------------------- | 
| 10000             |              0.000050 s           |            0.000054 s            | 
| 100000            |              0.000514 s           |            0.000413 s            | 
| 1000000           |              0.003767 s           |            0.002902 s            | 
| 10000000          |              0.040856 s           |            0.030603 s            | 
| 100000000         |              0.537259 s           |            0.324867 s            |


## 3. Atminties perskirstymai

| Elementų skaičius |   Perskirstymų skaičius naudojant std::vector    |   Perskirstymų skaičius naudojant MyVector      |
| ----------------  | ------------------------------------------------ | ----------------------------------------------- | 
| 10000             |                        18                        |                       11                        | 
| 100000            |                        24                        |                       15                        | 
| 1000000           |                        30                        |                       18                        | 
| 10000000          |                        36                        |                       21                        | 
| 100000000         |                        41                        |                       25                        |


## 4. Efektyvumo/spartos analizę ( v2.0 )

1000000 elementų:

|     Vektoriai    |  Nuskaitymo laikas | Skaičiavimas laikas | Rūšiavimo laikas | Įrašimo į buferį laikas | Pilnas įrašimo laikas |
| ---------------- |  ----------------- | ------------------- | ---------------- | ----------------------- | --------------------- |
| std::vector      |   4.517661 s       |   0.019548 s        |   2.915349 s     |  0.807474 s             |    0.911079 s         |
| MyVector         |     4.357800 s     |    0.015514 s       | 2.680118 s       |   0.822100 s            |    0.925719 s         |


