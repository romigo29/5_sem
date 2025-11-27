#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Task {
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining;

    int start = -1;
    int finish = 0;
    int wait = 0;
    int tat = 0;
};

struct Slice {
    int id;
    int start;
    int end;
};

int main() {

    setlocale(LC_ALL, "ru");

    int quantum;
    cout << "Введите квант (время кванта для вытеснения): ";
    cin >> quantum;

    vector<Task> tasks = {
        {1,0,80,3},{2,20,45,5},{3,30,60,2},{4,50,30,4},{5,70,90,1},
        {6,100,55,3},{7,120,75,2},{8,150,40,5},{9,180,65,4},{10,200,85,1}
    };

    int n = tasks.size();
    for (auto& t : tasks) t.remaining = t.burst;

    int time = 0;
    int completed = 0;
    vector<Slice> plan;

    while (completed < n) {
        int best = -1;
        int bestPr = INT_MIN;

        for (int i = 0; i < n; i++) {
            if (tasks[i].arrival <= time && tasks[i].remaining > 0) {
                if (tasks[i].priority > bestPr) {
                    bestPr = tasks[i].priority;
                    best = i;
                }
            }
        }

        if (best == -1) {
            time++;
            continue;
        }

        if (tasks[best].start == -1) {
            tasks[best].start = time;
        }

        int exec = min(quantum, tasks[best].remaining);
        int sliceStart = time;
        tasks[best].remaining -= exec;
        time += exec;
        int sliceEnd = time;

        plan.push_back({ tasks[best].id, sliceStart, sliceEnd });

        if (tasks[best].remaining == 0) {
            tasks[best].finish = time;
            tasks[best].tat = tasks[best].finish - tasks[best].arrival;
            tasks[best].wait = tasks[best].tat - tasks[best].burst;
            completed++;
        }
    }

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

    cout << "\n=== Preemptive Priority (Q = " << quantum << ") ===\n";
    cout << "План выполнения (диаграмма Ганта):\n";
    for (auto& s : plan) {
        cout << "Задача " << s.id << ": [" << s.start << " - " << s.end << "]\n";
    }

    cout << fixed << setprecision(2);
 
    cout << "Среднее время ожидания:      " << avgWait << "\n";
    cout << "Среднее время оборота:       " << avgTat << "\n";
    cout << "Среднее время выполнения:    " << avgBurst << "\n";
    cout << "Пропускная способность:      " << throughput << "\n";

    return 0;
}
