#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
bool gen_create = false;
bool gen_init = false;
int size;
int* mt = nullptr;
int** graph = nullptr;
bool* used = nullptr;
FILE* READ;
FILE* WRITE;
int** create_mass(int size) {
    gen_create = true;
    int** n = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++) {
        n[i] = (int*)malloc(sizeof(int) * size);
    }
    return n;
}

int** initialize(int** n, int size) {
    gen_init = true;
    srand(time(NULL));
    bool hasEdge = false;
    int mid = size / 2;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            n[i][j] = 0; 
        }
    }
    for (int i = 0; i < mid; i++) {
        for (int j = mid; j < size; j++) {
            if (rand() % 2) { 
                n[i][j] = 1;
                n[j][i] = 1; 
                hasEdge = true;
            }
        }
    }
    if (!hasEdge) {
        cout << "Граф несмежный (нет рёбер)!" << endl;
        return nullptr;
    }
    FILE* WRITE = fopen("Write_graph.txt", "w");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(WRITE, "%d ", n[i][j]);
        }
        fprintf(WRITE, "\n");
    }
    fclose(WRITE);
    return n;
}
int ruchn_vvod(int** n, int size) {
    gen_init = true;
    cout << "Введите граф размером " << size * size << " символов (только 0 и 1):\n";
    bool hasEdge = false;  
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cin >> n[i][j];
            while (n[i][j] != 0 && n[i][j] != 1) {
                cout << "Некорректный ввод! Введите 0 или 1: ";
                cin >> n[i][j];
            }
            if (n[i][j] == 1) {
                hasEdge = true;  
            }
        }
    }
    if (!hasEdge) {
        cout << "Граф несмежный (нет рёбер)!" << endl;
    }
    return 0;
}
int** file_vvod(int size, int** n) {
    gen_init = true;
    FILE* READ = fopen("Read_graph.txt", "r");
    if (READ == NULL) {
        cout << "Файл не найден!" << endl;
        return nullptr;
    }
    fseek(READ, 0, SEEK_END);
    long size_file = ftell(READ);
    fseek(READ, 0, SEEK_SET);

    if (size_file == size * size) {
        bool hasEdge = false; 
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int value = fgetc(READ) - '0';
                if (value >= 0 && value <= 1) {
                    n[i][j] = value;
                    if (value == 1) {
                        hasEdge = true; 
                    }
                }
                else {
                    cout << "Некорректный символ!" << endl;
                    fclose(READ);
                    return nullptr;
                }
            }
        }

       
        if (!hasEdge) {
            cout << "Граф несмежный (нет рёбер)!" << endl;
            fclose(READ);
            return nullptr;
        }
    }
    else {
        cout << "Размеры не совпадают!" << endl;
        fclose(READ);
        return nullptr;
    }

    fclose(READ);
    return n;
}


bool khun(int v, int size) {
    if (used[v])
        return false;
    used[v] = true;
    for (int to = 0; to < size; ++to) {
        if (graph[v][to] && (mt[to] == -1 || khun(mt[to], size))) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

int** generate_spisok(int size, int** n) {
    int** resultBipartiteGraph = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++) {
        resultBipartiteGraph[i] = (int*)malloc(sizeof(int) * size);
        for (int j = 0; j < size; j++) {
            resultBipartiteGraph[i][j] = n[i][j];
        }
    }
    return resultBipartiteGraph;
}

int main() {
    int** n = nullptr;
    setlocale(LC_ALL, "");
    int k = 1;
    int size, k_new;
    while (k) {
        cout << "\n1 - Задать двудольный граф размером NхN\n2 - Заполнить граф автоматически\n3 - Заполнить граф вручную(из файла или с клавиатуры)\n4 - Вывод сгенерированного графа\n5 - Алгоритм Куна\n"
            << "0 - выход" << endl;
        cin >> k;

        if (k == 1) {
            cout << "Введите количество вершин (больше 2): ";
            cin >> size;
            if (size > 2) {
                n = create_mass(size);
                if (n)
                    cout << "Граф создан!\n";
                else
                    cout << "Ошибка распределения памяти!\n";
            }
            else {
                cout << "Введите корректные данные!\n";
            }
        }

 
        if (k == 2) {
            if (gen_create) {
                initialize(n, size);
                cout << "Граф заполнен и записан в файл!\n";
            }
            else {
                cout << "Задайте размер графа!\n";
            }
        }
   
        if (k == 3) {
            cout << "\n1 - Задать граф с клавиатуры\n2 - Задать граф из файла в папке проекта\n";
            cin >> k_new;
            if ((k_new > 2) || (k_new < 1)) {
                cout << "Введите корректные данные!\n";
            }
            else if (k_new == 1) {
                if (gen_create) {
                    ruchn_vvod(n, size);
                }
                else {
                    cout << "Задайте размер графа!\n";
                }
            }
            else if (k_new == 2) {
                if (gen_create) {
                    n = file_vvod(size, n);
                }
                else {
                    cout << "Задайте размер графа!\n";
                }
            }
        }
 
        if (k == 4) {
            if (gen_create && gen_init) {
                cout << "Сгенерированный граф: \n";
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        printf("%3d", n[i][j]);
                    }
                    cout << "\n";
                }
            }
            else {
                cout << "Сгенерируйте граф!\n";
            }
        }

        if (k == 5) {
            if (gen_create && gen_init) {
                WRITE = fopen("Write_graph.txt", "a+");
                fprintf(WRITE, "Наибольшее паросочетание\n");
                cout << "Наибольшее паросочетание:\n";

                graph = generate_spisok(size, n);
                mt = (int*)malloc(size * sizeof(int));
                used = (bool*)malloc(size * sizeof(bool));
                for (int i = 0; i < size; ++i) mt[i] = -1;

                for (int v = 0; v < size; ++v) {
                    for (int i = 0; i < size; ++i) used[i] = false;
                    khun(v, size);
                }

                for (int i = 0; i < size; ++i) {
                    if (mt[i] != -1) {
                        printf("%d %d\n", mt[i], i);
                        fprintf(WRITE, "%d %d\n", mt[i], i);
                    }
                }

                fclose(WRITE);
                free(mt);
                free(used);
                for (int i = 0; i < size; ++i) free(graph[i]);
                free(graph);
            }
            else {
                cout << "Сгенерируйте граф!\n";
            }
        }
        if (k == 0)
            return 0;
    }
    system("pause");
}
