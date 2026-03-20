// syscalls.c
#include <errno.h>
#include <sys/stat.h>

// Внешние переменные, определённые в скрипте линкера
extern char _end[];        // Конец статических данных (из .ld)
extern char _heap_start[]; // Начало кучи (из .ld)
extern char _heap_end[];   // Конец кучи (из .ld)

/**
 * Увеличивает размер кучи.
 * \param incr Количество байт, на которое нужно увеличить кучу.
 * \return Указатель на предыдущую границу кучи, или -1 при ошибке.
 */
void *_sbrk(ptrdiff_t incr) {
    static char *current_heap_end = NULL;
    char *previous_heap_end;

    // Инициализация при первом вызове
    if (current_heap_end == NULL) {
        current_heap_end = _heap_start; // Начинаем с начала кучи
    }

    previous_heap_end = current_heap_end;

    // Проверяем, не выйдем ли мы за пределы кучи
    if (current_heap_end + incr > _heap_end) {
        // Ошибка: недостаточно памяти в куче
        errno = ENOMEM;
        return (void *) -1; // Возвращаем -1, как требует спецификация
    }

    // Сдвигаем границу
    current_heap_end += incr;

    return (void *) previous_heap_end;
}