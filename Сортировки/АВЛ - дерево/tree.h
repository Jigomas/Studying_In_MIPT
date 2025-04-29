#ifndef _TREE_H_
#define _TREE_H_

/** Интерфейс дерева */
typedef struct _tree Tree;

/** Функция, которая будет вызвана при добавлении элемента в дерево.
 * Дерево может "клонировать" элемент себе. 
 */
typedef void* (*tree_clone)(const void*);

/**  Функция, которая будет вызвана для сравнения элементов дерева.
 * Должна работать аналогично strcmp:
 * Возвращать -1 (или любое отрицательное число),
 * в случае, если первый элемент меньше второго,
 * +1 (или положительное), если первый больше второго 
 * и 0, если элементы равны (эквивалентны с точки зрения порядка).
 */
typedef int   (*tree_cmp)(const void*, const void*);
/** Функция, которая будет вызвана при освобожднении склонированного
 * элемента дерева
 */
typedef void  (*tree_free)(void *);
/** Функция, котороая убдет вызвана при прохождении дерева.
 * Например, в tree_traverse
 */
typedef int  (*tree_cb)(Tree* t, int level, const void*);

/* Creates a new tree with interface functions */
Tree* tree_create(tree_clone, tree_cmp, tree_free);

/* Inserts a new element into tree */
int tree_insert(Tree* t, const void* );

/* Searches for an element */
int tree_search(Tree* t, const void* key, void** value);

/* Deletes element from the tree */
int tree_delete(Tree* t, const void* key);

/* Tree destroy */
void tree_destroy(Tree* t);


/* Tree traverse */

int tree_traverse(Tree* t, const void* start, tree_cb cb);

#endif /*  _TREE_H_ */
