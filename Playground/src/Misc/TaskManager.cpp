#include "TaskManager.h"

#include <iostream>

std::vector<TaskManager::Task> TaskManager::tasks;
float TaskManager::lastDelta;

void TaskManager::schedule(std::function<void()> task, TaskType type, float info) {
    Task t;
    t.task = task;
    t.type = type;
    t.info = info;
    t.scheduledAt = CV::time();
    tasks.push_back(t);
}


void TaskManager::update() {
    for (auto it = tasks.begin(); it != tasks.end(); ) {
        Task& t = *it;
        float now = CV::time();
        float elapsed = now - t.scheduledAt;
        if (t.type == TaskType::AFTER) {
            // executa depois de n segundos
            if (elapsed >= t.info) {
                t.task();
                it = tasks.erase(it);
            } else {
                it++;
            }
        } else if (t.type == TaskType::REPEATING) {
            // executa ela de n em n segundos
            if (elapsed >= t.info) {
                t.task();
                t.scheduledAt = now;
            }
            it++;
        } else if (t.type == TaskType::DURATION) {
            // executa o maximo de vezes durante n segundos
            if (elapsed >= t.info) {
                it = tasks.erase(it);
            } else {
                t.task();
                it++;
            }
        } else if (t.type == TaskType::AMOUNT) {
            // executa n vezes
            if (t.info >= 0) {
                t.task();
                t.info -= 1.0f;
            } else {
                it = tasks.erase(it);
            }
        }
    }
}

