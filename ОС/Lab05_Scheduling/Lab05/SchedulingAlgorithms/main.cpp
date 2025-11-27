#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

struct Task {
    int id;
    int arrival;
    int burst;
    int priority;

    // Для расчётов
    int remaining;
    int start = -1;
    int finish = 0;
    int wait = 0;
    int turnaround = 0;
};

struct Stats {
    double avg_wait;
    double avg_burst;
    double avg_tat;
    double throughput;
};

void printStats(const Stats& s, const string& name) {
    cout << "\n=== " << name << " ===\n";
    cout << "Среднее время ожидания:      " << s.avg_wait << endl;
    cout << "Среднее время выполнения:    " << s.avg_burst << endl;
    cout << "Среднее время оборота:       " << s.avg_tat << endl;
    cout << "Пропускная способность:      " << s.throughput << endl;
}

// =====================================================
//                 Shortest Job First (SJF)
// =====================================================
Stats SJF(vector<Task> tasks) {
    int n = tasks.size();
    for (auto& t : tasks) t.remaining = t.burst;

    sort(tasks.begin(), tasks.end(), [](auto& a, auto& b) {
        return a.arrival < b.arrival;
        });

    int time = 0, completed = 0;
    vector<bool> finished(n, false);

    while (completed < n) {
        int idx = -1;
        int minburst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!finished[i] && tasks[i].arrival <= time) {
                if (tasks[i].burst < minburst) {
                    minburst = tasks[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        tasks[idx].start = time;
        time += tasks[idx].burst;
        tasks[idx].finish = time;
        tasks[idx].turnaround = tasks[idx].finish - tasks[idx].arrival;
        tasks[idx].wait = tasks[idx].turnaround - tasks[idx].burst;

        finished[idx] = true;
        completed++;
    }

    Stats s;
    s.avg_wait = 0; s.avg_burst = 0; s.avg_tat = 0;

    for (auto& t : tasks) {
        s.avg_wait += t.wait;
        s.avg_burst += t.burst;
        s.avg_tat += t.turnaround;
    }

    s.avg_wait /= n;
    s.avg_burst /= n;
    s.avg_tat /= n;

    double total_time = tasks.back().finish;
    s.throughput = n / total_time;

    return s;
}

// =====================================================
//                     Round Robin
// =====================================================
Stats RoundRobin(vector<Task> tasks, int quantum) {
    int n = tasks.size();
    for (auto& t : tasks) t.remaining = t.burst;

    queue<int> q;
    int time = 0, completed = 0;

    vector<bool> inQueue(n, false);
    int nextTask = 0;

    while (completed < n) {

        while (nextTask < n && tasks[nextTask].arrival <= time) {
            q.push(nextTask);
            inQueue[nextTask] = true;
            nextTask++;
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int i = q.front();
        q.pop();

        if (tasks[i].start == -1)
            tasks[i].start = time;

        int exec = min(quantum, tasks[i].remaining);
        tasks[i].remaining -= exec;
        time += exec;

        while (nextTask < n && tasks[nextTask].arrival <= time) {
            q.push(nextTask);
            inQueue[nextTask] = true;
            nextTask++;
        }

        if (tasks[i].remaining > 0) {
            q.push(i);
        }
        else {
            tasks[i].finish = time;
            tasks[i].turnaround = tasks[i].finish - tasks[i].arrival;
            tasks[i].wait = tasks[i].turnaround - tasks[i].burst;
            completed++;
        }
    }

    Stats s;
    s.avg_wait = 0; s.avg_burst = 0; s.avg_tat = 0;

    for (auto& t : tasks) {
        s.avg_wait += t.wait;
        s.avg_burst += t.burst;
        s.avg_tat += t.turnaround;
    }

    s.avg_wait /= n;
    s.avg_burst /= n;
    s.avg_tat /= n;

    double total_time = tasks.back().finish;
    s.throughput = n / total_time;

    return s;
}

// =====================================================
//             Preemptive Priority Scheduling
// =====================================================
Stats PriorityPreemptive(vector<Task> tasks, int quantum) {
    int n = tasks.size();
    for (auto& t : tasks) t.remaining = t.burst;

    int time = 0, completed = 0, nextTask = 0;

    while (completed < n) {

        int best = -1;
        int maxPr = -1;

        for (int i = 0; i < n; i++) {
            if (tasks[i].arrival <= time && tasks[i].remaining > 0) {
                if (tasks[i].priority > maxPr) {
                    maxPr = tasks[i].priority;
                    best = i;
                }
            }
        }

        if (best == -1) {
            time++;
            continue;
        }

        if (tasks[best].start == -1)
            tasks[best].start = time;

        int exec = min(quantum, tasks[best].remaining);

        tasks[best].remaining -= exec;
        time += exec;

        if (tasks[best].remaining == 0) {
            tasks[best].finish = time;
            tasks[best].turnaround = tasks[best].finish - tasks[best].arrival;
            tasks[best].wait = tasks[best].turnaround - tasks[best].burst;
            completed++;
        }
    }

    Stats s{};
    for (auto& t : tasks) {
        s.avg_wait += t.wait;
        s.avg_burst += t.burst;
        s.avg_tat += t.turnaround;
    }
    s.avg_wait /= n;
    s.avg_burst /= n;
    s.avg_tat /= n;

    double total_time = tasks.back().finish;
    s.throughput = n / total_time;

    return s;
}

// =====================================================
//                      MAIN
// =====================================================
int main() {

    setlocale(LC_ALL, "ru");

    vector<Task> input = {
        {1,0,80,3},
        {2,20,45,5},
        {3,30,60,2},
        {4,50,30,4},
        {5,70,90,1},
        {6,100,55,3},
        {7,120,75,2},
        {8,150,40,5},
        {9,180,65,4},
        {10,200,85,1}
    };

    cout << fixed << setprecision(2);

    // SJF
    Stats sjf = SJF(input);
    printStats(sjf, "SJF");

    // RR (попробуем квант 10)
    int quantum = 10;
    Stats rr = RoundRobin(input, quantum);
    printStats(rr, "Round Robin (Q = 10)");

    // RR (попробуем квант 5)
    quantum = 5;
    rr = RoundRobin(input, quantum);
    printStats(rr, "Round Robin (Q = 5");

    // RR (попробуем квант 20)
    quantum = 20;
    rr = RoundRobin(input, quantum);
    printStats(rr, "Round Robin (Q = 20)");


    // Priority Preemptive (квант 10)
    Stats pr = PriorityPreemptive(input, quantum);
    printStats(pr, "Preemptive Priority (Q = 10)");


    // Priority Preemptive (квант 5)
    pr = PriorityPreemptive(input, quantum);
    printStats(pr, "Preemptive Priority (Q = 5)");


    // Priority Preemptive (квант 20)
    pr = PriorityPreemptive(input, quantum);
    printStats(pr, "Preemptive Priority (Q = 20)");

    return 0;
}
