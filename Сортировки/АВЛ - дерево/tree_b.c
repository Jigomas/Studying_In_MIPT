#include "tree.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct _node {
	void* v;
	struct _node* l;
	struct _node* r;
	int h;
} Node;


struct _tree {
	Node* root;
	tree_clone clone;
	tree_cmp   cmp;
	tree_free free;
};

#define H(x) ((x)?(x)->h:0)
#define MAX(a, b) ((a)<(b)?(b):(a))
#define DROP(x) (x)

Tree* tree_create(tree_clone clone,
		tree_cmp cmp,
		tree_free free) {
	Tree* t = malloc(sizeof(Tree));
	if ( !t ) return NULL;
	*t = (Tree){NULL, clone, cmp, free};
	return t;
}



Node* _Node(Node* L, void* v, Node* R, Node* old)  {
	if ( !old) old = malloc(sizeof(Node));
	if (!old) return NULL;
	*old = (Node){v, L, R, 1 + MAX(H(L),H(R))};
	return old;
}

Node* BT(Node* L, void* v, Node* R, Node* old) {
	if(H(R)-H(L) == 2) {
		if ( H(R->r) > H(R->l)) return _Node(_Node(L, v, R->l, DROP(old)), R->v, R->r, DROP(R));
		Node* RL = R->l;
		return _Node(_Node(L, v, RL->l, DROP(old)), RL->v, _Node(RL->r, R->v, R->r, DROP(R)), DROP(RL));
	} else if ( H(L)-H(R) == 2 ) {
		if ( H(L->l) > H(L->r)) return _Node(L->l, L->v, _Node(L->r, v, R, DROP(old)), DROP(L));
		Node* LR = L->r;
		return _Node(_Node(L->l, L->v, LR->l, DROP(L)), LR->v, _Node(LR->r, v, R, DROP(old)), DROP(LR));
	}
	return _Node(L, v, R, DROP(old));
}


Node* _tree_insert(Tree* t, Node* n, const void* pair) {
	if ( !n) return _Node(NULL, t->clone?t->clone(pair):(void*)pair, NULL, DROP(NULL));
	if ( t->cmp(pair, n->v) > 0) return BT(n->l, n->v, _tree_insert(t, n->r, pair), DROP(n));
	return BT(_tree_insert(t, n->l, pair), n->v, n->r, DROP(n));
}

int tree_insert(Tree* t, const void* pair) {
	t->root =  _tree_insert(t, t->root, pair);
	return 0;
}

static int _tree_search(Tree* t, Node* n, const void* key, void** value) {
	if ( !n ) return 0;
	int cmp = t->cmp(key, n->v);
	if ( cmp == 0 ) {
		*value = n->v;
		return 1;
	}
	if ( cmp > 0 )
		return _tree_search(t, n->r, key, value);
	else
		return _tree_search(t, n->l, key, value);
}

int tree_search(Tree* t, const void* key, void** value) {
	return _tree_search(t, t->root, key, value); 
}

int _tree_traverse(Tree* t, int level, const void* start, Node* n, tree_cb cb) {
	if ( !n ) return 0;
	if ( !start || t->cmp(start, n->v) <= 0 ) {
		if (_tree_traverse(t, level+1, start, n->l, cb)) return 1;
		if ( cb && cb(t, level, n->v) ) return 1;
	}
	return _tree_traverse(t, level+1, start, n->r, cb);
}


Node* _tree_delete(Tree* t, Node* L, void* v, Node* R,  const void* key, Node* old) {
	int cmp = t->cmp(key, v);
	if ( cmp == 0 ) {
		if ( !L && !R ) {
			if ( t->free) t->free(v);
			if ( old) free(old);
			return NULL;
		}
		if( H(R) > H(L) ) return BT(_tree_delete(t, L, v, R->l, key, DROP(old)), R->v, R->r, DROP(R));
		else return L?BT(L->l, L->v, _tree_delete(t, L->r, v, R, key, DROP(old)), DROP(L)):NULL;
	} else if ( cmp > 0) return R?BT(L, v, _tree_delete(t, R->l, R->v, R->r, key, DROP(old)), DROP(R)):NULL;
	else return L?BT(_tree_delete(t, L->l, L->v, L->r, key, DROP(old)), v, R, DROP(L)):NULL;
}


int tree_delete(Tree* t, const void* key) {
	t->root = t->root?_tree_delete(t, t->root->l, t->root->v, t->root->r, key, DROP(t->root)):NULL;
	return 0;
}


int tree_traverse(Tree* t, const void* start, tree_cb cb ) {
	return _tree_traverse(t, 0, start, t->root, cb);
}

void _tree_destroy(Tree* t, Node* n) {
	if ( !n ) return;
	_tree_destroy(t, n->l);
	_tree_destroy(t, n->r);
	if ( t->free ) t->free(n->v);
	free(n);
}


void tree_destroy(Tree* t) {
	_tree_destroy(t, t->root);
}

