#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Task {
    int id;
    int arrival;
    int burst;

    int start = -1;
    int finish = 0;
    int wait = 0;
    int tat = 0;
};

// Элемент для записи плана выполнения
struct Slice {
    int id;
    int start;
    int end;
};

int main() {

    setlocale(LC_ALL, "ru");

    vector<Task> tasks = {
        {1,0,80},{2,20,45},{3,30,60},{4,50,30},{5,70,90},
        {6,100,55},{7,120,75},{8,150,40},{9,180,65},{10,200,85}
    };
    int n = tasks.size();

    // Сортируем задачи по времени поступления
    sort(tasks.begin(), tasks.end(), [](auto& a, auto& b) {
        return a.arrival < b.arrival;
        });

    int time = 0;
    int completed = 0;
    vector<bool> done(n, false);
    vector<Slice> plan;  // план выполнения

    while (completed < n) {
        int idx = -1;
        int bestBurst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!done[i] && tasks[i].arrival <= time) {
                if (tasks[i].burst < bestBurst) {
                    bestBurst = tasks[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // Процессор простаивает
            time++;
            continue;
        }

        // Запоминаем начало среза
        int sliceStart = time;

        // Запускаем задачу полностью
        tasks[idx].start = time;
        time += tasks[idx].burst;
        tasks[idx].finish = time;

        // Фиксируем срез
        plan.push_back({ tasks[idx].id, sliceStart, time });

        // Рассчитываем времена ожидания и оборота
        tasks[idx].tat = tasks[idx].finish - tasks[idx].arrival;
        tasks[idx].wait = tasks[idx].tat - tasks[idx].burst;

        done[idx] = true;
        completed++;
    }

    // Вычисляем статистику
    double sumWait = 0, sumTat = 0, sumBurst = 0;
    for (auto& t : tasks) {
        sumWait += t.wait;
        sumTat += t.tat;
        sumBurst += t.burst;
    }
    double avgWait = sumWait / n;
    double avgTat = sumTat / n;
    double avgBurst = sumBurst / n;
    double throughput = n / (double)tasks.back().finish;

    cout << "\n=== SJF ===\n";
    // Вывод плана выполнения
    cout << "\nПлан выполнения (диаграмма Ганта):\n";
    for (auto& s : plan) {
        cout << "Задача " << s.id << ": [" << s.start << " - " << s.end << "]\n";
    }

    // Вывод статистики
    cout << fixed << setprecision(2);
    
    cout << "Среднее время ожидания:      " << avgWait << "\n";
    cout << "Среднее время оборота:       " << avgTat << "\n";
    cout << "Среднее время выполнения:    " << avgBurst << "\n";
    cout << "Пропускная способность:      " << throughput << "\n";

    return 0;
}
