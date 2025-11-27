#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

struct Task {
    int id;
    int arrival;
    int burst;
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
    cout << "Введите квант: ";
    cin >> quantum;

    vector<Task> tasks = {
        {1,0,80},{2,20,45},{3,30,60},{4,50,30},{5,70,90},
        {6,100,55},{7,120,75},{8,150,40},{9,180,65},{10,200,85}
    };
    int n = tasks.size();
    for (auto& t : tasks) t.remaining = t.burst;

    queue<int> q;
    int time = 0;
    int completed = 0;
    int next = 0;

    vector<Slice> plan;

    while (completed < n) {
        while (next < n && tasks[next].arrival <= time) {
            q.push(next);
            next++;
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int i = q.front();
        q.pop();

        if (tasks[i].start == -1) {
            tasks[i].start = time;
        }

        int exec = min(quantum, tasks[i].remaining);
        int sliceStart = time;
        time += exec;
        tasks[i].remaining -= exec;
        int sliceEnd = time;

        plan.push_back({ tasks[i].id, sliceStart, sliceEnd });

        while (next < n && tasks[next].arrival <= time) {
            q.push(next);
            next++;
        }

        if (tasks[i].remaining > 0) {
            q.push(i);
        }
        else {
            tasks[i].finish = time;
            tasks[i].tat = tasks[i].finish - tasks[i].arrival;
            tasks[i].wait = tasks[i].tat - tasks[i].burst;
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

    cout << "=== Round Robin (Q = " << quantum << ") ===\n";
    cout << "\nПлан выполнения (диаграмма Ганта):\n";
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
