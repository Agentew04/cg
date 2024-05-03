#ifndef __TIME_RENDERER_H__
#define __TIME_RENDERER_H__

#include <functional>
#include <vector>

#include "../gl_canvas2d.h"

/// @brief Classe que gerencia tarefas a serem executadas em um tempo arbitrario.
/// Basicamente roda um lambda varias vezes em um intervalo de tempo. Simula
/// um async meia boca usando o game loop.
class TaskManager{
public:

    enum class TaskType {
        /// @brief Executa a tarefa uma vez depois de n segundos
        AFTER,
        /// @brief Executa a tarefa repetidas vezes com n segundos de intervalo
        REPEATING,
        /// @brief Executa a tarefa o maximo de vezes possiveis em n segundos
        DURATION,
        /// @brief Executa a tarefa n vezes
        AMOUNT
    };

    struct Task {
        std::function<void()> task;
        TaskType type;
        float info;
        float scheduledAt;
    };

    /// @brief Agenda uma tarefa a ser executada arbitrarias vezes
    /// @param task A tarefa a ser executada
    /// @param type O tipo de execucao da tarefa. Ver TaskType
    /// @param info O valor associado da tarefa.
    static void schedule(std::function<void()> task, TaskType type, float info);

    /// @brief Executa as tarefas que devem ser executadas
    static void update();
private:
    
    static float lastDelta;
    static std::vector<Task> tasks;
};

#endif // __TIME_RENDERER_H__