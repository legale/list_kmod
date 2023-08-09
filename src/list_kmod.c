#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

// Структура для элемента списка
struct my_list_entry {
  int data;
  struct list_head list;
};

// Инициализация списка
LIST_HEAD(my_list);

// Прототипы функций
void add_to_beginning(int data);
void add_to_end(int data);
void remove_from_beginning(void);
void remove_from_end(void);
void remove_element(int data);
void clear_list(void);
void demo_linked_list(void);

// Создать элемент списка и добавить его в начало
void add_to_beginning(int data) {
  struct my_list_entry *new_entry = kmalloc(sizeof(struct my_list_entry), GFP_KERNEL);
  new_entry->data = data;
  list_add(&new_entry->list, &my_list);
}

// Добавить элемент в конец списка
void add_to_end(int data) {
  struct my_list_entry *new_entry = kmalloc(sizeof(struct my_list_entry), GFP_KERNEL);
  new_entry->data = data;
  list_add_tail(&new_entry->list, &my_list);
}

// Удалить элемент из начала списка
void remove_from_beginning() {
  if (!list_empty(&my_list)) {
    struct my_list_entry *entry = list_first_entry(&my_list, struct my_list_entry, list);
    list_del(&entry->list);
    kfree(entry);
  }
}

// Удалить элемент с конца списка
void remove_from_end() {
  if (!list_empty(&my_list)) {
    struct my_list_entry *entry = list_last_entry(&my_list, struct my_list_entry, list);
    list_del(&entry->list);
    kfree(entry);
  }
}

// Удалить произвольный элемент списка
void remove_element(int data) {
  struct my_list_entry *entry;
  struct list_head *pos, *tmp;

  list_for_each_safe(pos, tmp, &my_list) {
    entry = list_entry(pos, struct my_list_entry, list);
    if (entry->data == data) {
      list_del(pos);
      kfree(entry);
      break;
    }
  }
}

// Очистить список
void clear_list() {
  struct my_list_entry *entry, *tmp;

  list_for_each_entry_safe(entry, tmp, &my_list, list) {
    list_del(&entry->list);
    kfree(entry);
  }
}

// Функция для демонстрации работы со списком
void demo_linked_list() {
  struct my_list_entry *entry;
  add_to_beginning(1);
  add_to_end(2);
  add_to_end(3);
  add_to_end(4);

  printk("list after add elements: ");
  list_for_each_entry(entry, &my_list, list) {
    printk("%d ", entry->data);
  }
  printk("\n");

  remove_from_beginning();

  printk("list after del first element: ");
  list_for_each_entry(entry, &my_list, list) {
    printk("%d ", entry->data);
  }
  printk("\n");

  remove_from_end();

  printk("list after del last element: ");
  list_for_each_entry(entry, &my_list, list) {
    printk("%d ", entry->data);
  }
  printk("\n");

  remove_element(2);

  printk("list after delete element 2: ");
  list_for_each_entry(entry, &my_list, list) {
    printk("%d ", entry->data);
  }
  printk("\n");

  clear_list();
}

int __init kmod_init(void) {
  printk("demo:\n");
  demo_linked_list();
  return 0;
}

void __exit kmod_deinit(void) {
  printk("module unloaded.\n");
}

module_init(kmod_init);
module_exit(kmod_deinit);

MODULE_LICENSE("MIT");
MODULE_DESCRIPTION("Simple linked list kernel module");
MODULE_AUTHOR("me");