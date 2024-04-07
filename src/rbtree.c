#include "rbtree.h"

#include <stdlib.h>

node_t* bt_insert(node_t*, node_t*);
node_t* rbtree_grapnd_parent(node_t*);
node_t* rbtree_uncle(node_t*);
void rotate_left(node_t*);
void rotate_right(node_t*);
void rbtree_insert1(node_t*);
void rbtree_insert2(node_t*);
void rbtree_insert3(node_t*);
void rbtree_insert4(node_t*);
void rbtree_insert5(node_t*);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* node = (node_t*) malloc(sizeof(node_t));
  node->color = RBTREE_RED;
  node->key = key;

  t->root = bt_insert(t->root, node);

  rbtree_insert1(node);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

node_t* bt_insert(node_t* node, node_t* new_node) {
  if (node == NULL) {
    return new_node;
  }

  if (node->key < new_node->key) {
    node->right = bt_insert(node->right, new_node);
  } else {
    node->left = bt_insert(node->left, new_node);
  }

  return node;
}

node_t* rbtree_grapnd_parent(node_t* node) {
  if (node == NULL) return NULL;
  if (node->parent == NULL) return NULL;

  return node->parent->parent;
}

node_t* rbtree_uncle(node_t* node) {
  node_t* grand_parent = rbtree_grapnd_parent(node);

  if (grand_parent == NULL) return NULL;

  if (grand_parent->left != node->parent) {
    return grand_parent->left;
  } else {
    return grand_parent->right;
  }
}

void rbtree_insert1(node_t* node) {
  if (node->parent == NULL) {
    node->color = RBTREE_BLACK;
  } else {
    rbtree_insert2(node);
  }
}

void rbtree_insert2(node_t* node) {
  if (node->parent->color == RBTREE_BLACK) {
    return;
  } else {
    rbtree_insert3(node);
  }
}

void rbtree_insert3(node_t* node) {
  node_t* uncle = rbtree_uncle(node);

  if (uncle != NULL && uncle->color == RBTREE_RED) {
    node->parent->color = RBTREE_BLACK;
    uncle->color = RBTREE_BLACK;
    node_t* grand_parent = rbtree_grapnd_parent(node);
    grand_parent->color = RBTREE_RED;
    rbtree_insert1(grand_parent);
  } else {
    rbtree_insert4(node);
  }
}

void rbtree_insert4(node_t* node) {
  node_t* grand_parent = rbtree_grapnd_parent(node);

  if (node == node->parent->right && node->parent == grand_parent->left) {
    rotate_left(node->parent);
    node = node->left;
  } else if (node == node->parent->left && node->parent == grand_parent->right) {
    rotate_right(node->parent);
    node = node->right;
  }

  rbtree_insert5(node);
}

void rbtree_insert5(node_t* node) {
  node_t* grand_parent = rbtree_grapnd_parent(node);

  node->parent->color = RBTREE_BLACK;
  grand_parent->color = RBTREE_RED;
  if (node == node->parent->left) {
    rotate_right(grand_parent);
  } else {
    rotate_left(grand_parent);
  }
}

void rotate_left(node_t* node) {
  node_t* child = node->right;
  node_t* parent = node->parent;

  if (child->left != NULL) {
    child->left->parent = node;
  }

  node->right = child->left;
  node->parent = child;
  child->left = node;
  child->parent = parent;

  if (parent != NULL) {
    if (parent->left == node) {
      parent->left = child;
    } else {
      parent->right = child;
    }
  }
}

void rotate_right(node_t* node) {
  node_t* child = node->left;
  node_t* parent = node->parent;

  if (child->right != NULL) {
    child->right->parent = node;
  }

  node->left = child->right;
  node->parent = child;
  child->right = node;
  child->parent = parent;

  if (parent != NULL) {
    if (parent->right == node) {
      parent->right = child;
    } else {
      parent->left = child;
    }
  }
}