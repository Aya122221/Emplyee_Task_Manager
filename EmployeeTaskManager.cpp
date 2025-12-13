#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

// ===================== Task Structure ============================
struct Task {
    string title;
    string description;
    string assignDate;
    string endDate;
    string employeeName;
    int priority; // 1 = least important, 10 = most important

    // For priority queue (max-heap)
    bool operator<(const Task& other) const {
        return priority < other.priority; // Higher priority first
    }
};

// ================ Linked List for Completed Tasks ==================
struct DoneNode {
    Task task;
    DoneNode* next;
    DoneNode(Task t) : task(t), next(nullptr) {}
};

class DoneList {
public:
    DoneNode* head = nullptr;

    void add(Task t) {
        DoneNode* node = new DoneNode(t);
        node->next = head;
        head = node;
    }

    void display() {
        cout << "\n===== Completed Tasks =====\n";
        DoneNode* curr = head;
        if (!curr) {
            cout << "No completed tasks yet.\n";
            return;
        }

        while (curr) {
            cout << "- " << curr->task.title << " (Priority: "
                << curr->task.priority << ")\n";
            curr = curr->next;
        }
    }
};

// ==================== Global Variables ============================
priority_queue<Task> taskQueue; // Sorted automatically
DoneList doneTasks;

// ==================== Helper Functions ============================

Task readTask() {
    Task t;

    cin.ignore();
    cout << "Task Title: ";
    getline(cin, t.title);

    cout << "Description: ";
    getline(cin, t.description);

    cout << "Assigning Date: ";
    getline(cin, t.assignDate);

    cout << "Required Ending Date: ";
    getline(cin, t.endDate);

    cout << "Employee Name: ";
    getline(cin, t.employeeName);

    cout << "Priority (1-10): ";
    cin >> t.priority;

    return t;
}

void displayTask(const Task& t) {
    cout << "\n--------------------------------------\n";
    cout << "Title: " << t.title << "\n";
    cout << "Description: " << t.description << "\n";
    cout << "Assign Date: " << t.assignDate << "\n";
    cout << "End Date: " << t.endDate << "\n";
    cout << "Employee: " << t.employeeName << "\n";
    cout << "Priority: " << t.priority << "\n";
}

void displayAll() {
    if (taskQueue.empty()) {
        cout << "\nNo tasks available.\n";
        return;
    }

    cout << "\n===== PRIORITY LIST (Top = Most Important) =====\n";

    priority_queue<Task> temp = taskQueue;
    while (!temp.empty()) {
        displayTask(temp.top());
        temp.pop();
    }
}

// Convert priority queue into vector for editing
vector<Task> queueToVector() {
    vector<Task> v;
    priority_queue<Task> temp = taskQueue;
    while (!temp.empty()) {
        v.push_back(temp.top());
        temp.pop();
    }
    return v;
}

void buildQueueFromVector(vector<Task>& v) {
    while (!taskQueue.empty()) taskQueue.pop();
    for (auto& t : v) taskQueue.push(t);
}

// ========================= Features ==============================

void addTask() {
    Task t = readTask();
    taskQueue.push(t);
    cout << "\nTask added successfully.\n";
}

void markTaskDone() {
    if (taskQueue.empty()) {
        cout << "\nNo tasks to complete.\n";
        return;
    }

    Task t = taskQueue.top();
    taskQueue.pop();

    cout << "\nTask Completed:\n";
    displayTask(t);

    doneTasks.add(t);
}

void editTask() {
    vector<Task> v = queueToVector();

    if (v.empty()) {
        cout << "No tasks to edit.\n";
        return;
    }

    cout << "\nEnter title of task to edit: ";
    cin.ignore();
    string title;
    getline(cin, title);

    bool found = false;

    for (auto& t : v) {
        if (t.title == title) {
            found = true;
            cout << "\nEditing Task...\n";
            displayTask(t);

            cout << "\nEnter new priority (1-10): ";
            cin >> t.priority;

            cout << "Task updated.\n";
            break;
        }
    }

    if (!found) {
        cout << "Task not found.\n";
        return;
    }

    buildQueueFromVector(v);
}

void postponeTask() {
    vector<Task> v = queueToVector();

    cout << "\nEnter task title to postpone: ";
    cin.ignore();
    string title;
    getline(cin, title);

    for (auto& t : v) {
        if (t.title == title) {
            cout << "Enter NEW required ending date: ";
            getline(cin, t.endDate);
            cout << "Task postponed.\n";
            buildQueueFromVector(v);
            return;
        }
    }

    cout << "Task not found.\n";
}

void nextTaskReminder() {
    if (taskQueue.empty()) {
        cout << "\nNo upcoming tasks.\n";
        return;
    }

    cout << "\n===== NEXT TASK TO DO =====\n";
    displayTask(taskQueue.top());
}

void encouragingQuote() {
    if (!taskQueue.empty()) return;

    cout << "\nAll tasks are done! 🎉\n";
    cout << "Quote: \"Success is the result of hard work every single day.\"\n";
}

// ============================ Menu ===============================

void menu() {
    int choice;

    do {
        cout << "\n========== EMPLOYEE TASK MANAGER ==========\n";
        cout << "1. Add Task\n";
        cout << "2. Display Sorted Priority List\n";
        cout << "3. Mark Next Task as Done\n";
        cout << "4. Edit Task Priority\n";
        cout << "5. Postpone Task\n";
        cout << "6. Next Task Reminder\n";
        cout << "7. Show Completed Tasks\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
        case 1: addTask(); break;
        case 2: displayAll(); break;
        case 3: markTaskDone(); encouragingQuote(); break;
        case 4: editTask(); break;
        case 5: postponeTask(); break;
        case 6: nextTaskReminder(); break;
        case 7: doneTasks.display(); break;
        case 0: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

// =========================== MAIN ================================

int main() {
    menu();
    return 0;
}
