#include "output_funcs.h"
#include <stdio.h>

extern int freed_zone;
extern int new_zone;

void print_menu()
{
    printf("\n\n\nМЕНЮ\nВведите действие (цифру), который хотите исполнить:\n"
           "0. Выход;\n"
           "1. Добавить элементы в стек, реализованный списком;\n"
           "2. Добавить элементы в стек, реализованный массивом;\n"
           "3. Удалить элементы из стека-списка;\n"
           "4. Удалить элементы из стека-массива;\n"
           "5. Вывести стек-список;\n"
           "6. Вывести стек-массив;\n"
           "7. Вывести массив свободных областей;\n"
           "8. Вывод общей статистики.\n"
           "\nВвод: ");
}

void print_free_zones(const array_d *const free_zones)
{
    printf("Количество элементов = %d;\n", free_zones->data_size);
    for (int i = 0; i < free_zones->data_size; i++)
        printf("%d. %p\n", i + 1, free_zones->data[i]);
}

void print_stack_l(const node_t *ps)
{
    if (ps == NULL)
    {
        printf("Стек пуст.\n");
        return;
    }
    printf("Стек, хранящийся в виде списка:\n");
    int i = 1;
    while (ps)
    {
        printf("%d. %p\n", i++, ps->value);
        ps = ps->next;
    }
}

void print_stack_a(stack_a *ps)
{
    if (ps->data == NULL)
    {
        printf("Стек пуст.\n");
        return;
    }
    int i = 1;
    printf("Стек, хранящийся в виде массива:\n");
    while (ps->last >= ps->data)
    {
        printf("%d. %p\n", i++, *ps->last);
        ps->last--;
    }
}

void print_stat()
{
    printf("Количество памяти, взятой по-новой: %d\n"
           "Количество памяти, взятой из уже использованной: %d",
           new_zone, freed_zone);
}