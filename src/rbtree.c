#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

node_t* bt_insert(node_t*, node_t*, node_t*);
node_t* bt_find(node_t*, const key_t);
void bt_delete(node_t*);

node_t* rbtree_root(node_t*);
node_t* rbtree_grapnd_parent(node_t*);
node_t* rbtree_uncle(node_t*);
node_t* rbtree_sibling(node_t*);

void rotate_left(node_t*);
void rotate_right(node_t*);
void rbtree_insert1(node_t*);
void rbtree_insert2(node_t*);
void rbtree_insert3(node_t*);
void rbtree_insert4(node_t*);
void rbtree_insert5(node_t*);

int rbtree_is_leaf(node_t*);
void rbtree_replace(node_t*, node_t*);
void delete_one_child(node_t*);
void rbtree_delete1(node_t*);
void rbtree_delete2(node_t*);
void rbtree_delete3(node_t*);
void rbtree_delete4(node_t*);
void rbtree_delete5(node_t*);
void rbtree_delete6(node_t*);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  p->root = NULL;
  p->nil = NULL;
  
  return p; 
}

void delete_rbtree(rbtree *t) {
  bt_delete(t->root);

  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t* node = (node_t*) malloc(sizeof(node_t));
  node->color = RBTREE_RED;
  node->key = key;

  t->root = bt_insert(NULL, t->root, node);

  rbtree_insert1(node);

  t->root = rbtree_root(node);

  return node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  return bt_find(t->root, key);
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
  if (p->left == NULL && p->right == NULL) {
    if (t->root == p) {
      t->root = NULL;
    }

    free(p);
  } else if (p->left == NULL && p->right != NULL) {
    if (t->root == p) {
      t->root = p->right;
    } else {
      rbtree_replace(p, p->right);
      p->right->color = RBTREE_BLACK;
    }

    free(p);
  } else if (p->left != NULL && p->right == NULL) {
    if (t->root == p) {
      t->root = p->right;
    } else {
      rbtree_replace(p, p->left);
      p->left->color = RBTREE_BLACK;
    }

    free(p);
  } else {
    delete_one_child(p);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

node_t* bt_insert(node_t* parent, node_t* node, node_t* new_node) {
  if (node == NULL) {
    new_node->parent = parent;
    return new_node;
  }

  if (node->key <= new_node->key) {
    node->right = bt_insert(node, node->right, new_node);
  } else {
    node->left = bt_insert(node, node->left, new_node);
  }

  return node;
}

node_t* bt_find(node_t* node, const key_t key) {
  if (node == NULL) return NULL;

  if (node->key == key) {
    return node;
  } else if (node->key <= key) {
    return bt_find(node->right, key);
  } else {
    return bt_find(node->left, key);
  }
}

void bt_delete(node_t* node) {
  if (node == NULL) return;

  bt_delete(node->left);
  bt_delete(node->right);
  free(node);
}

node_t* rbtree_root(node_t* node) {
  if (node == NULL) return NULL;

  if (node->parent != NULL) {
    return rbtree_root(node->parent);
  } else {
    return node;
  }
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

node_t* rbtree_sibling(node_t* node) {
  if (node == node->parent->left) {
    return node->parent->right;
  } else {
    return node->parent->left;
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

int rbtree_is_leaf(node_t* node) {
  if (node == NULL) return 1;
  
  return (node->left == NULL && node->right == NULL);
}

void rbtree_replace(node_t* node, node_t* child) {
  child->parent = node->parent;
  if (node->parent->left == node) {
    node->parent->left = child;
  } else if (node->parent->right == node) {
    node->parent->right = child;
  }
}

void delete_one_child(node_t* node) {
  node_t* child = rbtree_is_leaf(node->right) ? node->left : node->right;
  
  rbtree_replace(node, child);
  if (node->color == RBTREE_BLACK) {
    if (child->color == RBTREE_RED) {
      child->color = RBTREE_BLACK;
    } else {
      rbtree_delete1(child);
    }
  }

  free(node);
}

void rbtree_delete1(node_t* node) {
  if (node->parent != NULL) {
    rbtree_delete2(node);
  }
}

void rbtree_delete2(node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  if (sibling->color == RBTREE_RED) {
    node->parent->color = RBTREE_RED;
    sibling->color = RBTREE_BLACK;
    if (node == node->parent->left) {
      rotate_left(node->parent);
    } else {
      rotate_right(node->parent);
    }
  }
  
  rbtree_delete3(node);
}

void rbtree_delete3(node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  if (
    node->parent->color == RBTREE_BLACK
    && sibling->color == RBTREE_BLACK
    && sibling->left->color == RBTREE_BLACK
    && sibling->right->color == RBTREE_BLACK
  ) {
    sibling->color = RBTREE_RED;
    rbtree_delete1(node->parent);
  } else {
    rbtree_delete4(node);
  }
}

void rbtree_delete4(node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  if (
    node->parent->color == RBTREE_RED
    && sibling->color == RBTREE_BLACK
    && sibling->left->color == RBTREE_BLACK
    && sibling->right->color == RBTREE_BLACK
  ) {
    sibling->color = RBTREE_RED;
    node->parent->color = RBTREE_BLACK;
  } else {
    rbtree_delete5(node);
  }
}

void rbtree_delete5(node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  if (sibling->color == RBTREE_BLACK) {
    if (
      node == node->parent->left
      && sibling->right->color == RBTREE_BLACK
      && sibling->left->color == RBTREE_RED
    ) {
      sibling->color = RBTREE_RED;
      sibling->left->color = RBTREE_BLACK;
      rotate_right(sibling);
    } else if (
      node == node->parent->right
      && sibling->right->color == RBTREE_RED
      && sibling->left->color == RBTREE_BLACK
    ) {
      sibling->color = RBTREE_RED;
      sibling->right->color = RBTREE_BLACK;
      rotate_left(sibling);
    }
  }

  rbtree_delete6(node);
}

void rbtree_delete6(node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  sibling->color = node->parent->color;
  node->parent->color = RBTREE_BLACK;

  if (node == node->parent->left) {
    sibling->right->color = RBTREE_BLACK;
    rotate_left(node->parent);
  } else {
    sibling->left->color = RBTREE_BLACK;
    rotate_right(node->parent);
  }
}