#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

node_t* bt_insert(node_t*, node_t*, node_t*);
node_t* bt_find(node_t*, const key_t);
node_t* bt_max(node_t*);
node_t* bt_min(node_t*);
void bt_delete(node_t*);

node_t* rbtree_grand_parent(node_t*);
node_t* rbtree_uncle(node_t*);
node_t* rbtree_sibling(node_t*);

void rotate_left(rbtree*, node_t*);
void rotate_right(rbtree*, node_t*);
void rbtree_insert1(rbtree*, node_t*);
void rbtree_insert2(rbtree*, node_t*);
void rbtree_insert3(rbtree*, node_t*);
void rbtree_insert4(rbtree*, node_t*);
void rbtree_insert5(rbtree*, node_t*);

int rbtree_is_leaf(node_t*);
void rbtree_replace(node_t*, node_t*);
void delete_one_child(rbtree*, node_t*);
void rbtree_delete1(rbtree*, node_t*);
void rbtree_delete2(rbtree*, node_t*);
void rbtree_delete3(rbtree*, node_t*);
void rbtree_delete4(rbtree*, node_t*);
void rbtree_delete5(rbtree*, node_t*);
void rbtree_delete6(rbtree*, node_t*);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->root = NULL;
  p->nil = NULL;

  return p;
}

void delete_rbtree(rbtree *t) {
  if (t != NULL) {
    bt_delete(t->root);
    free(t);
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t* new_node = (node_t*) malloc(sizeof(node_t));
  if (new_node == NULL) return NULL;
  
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;

  if (t->root == NULL) {
      t->root = new_node;
      new_node->color = RBTREE_BLACK;
      return new_node;
  }

  t->root = bt_insert(NULL, t->root, new_node);

  rbtree_insert1(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  return bt_find(t->root, key);
}

node_t *rbtree_min(const rbtree *t) {
  return bt_min(t->root);
}

node_t *rbtree_max(const rbtree *t) {
  return bt_max(t->root);
}

int rbtree_erase(rbtree *t, node_t *p) {
  if (p->left == NULL && p->right == NULL) {
    if (t->root == p) {
      t->root = NULL;
    }

    free(p);
  } else {
    delete_one_child(t, p);
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

node_t* bt_max(node_t* node) {
  if (node == NULL) return NULL;

  while(node->right != NULL) {
    node = node->right;
  }
  
  return node;
}

node_t* bt_min(node_t* node) {
  if (node == NULL) return NULL;

  if(node->left == NULL) {
    return node;
  } else {
    return bt_min(node->left);
  }
}

void bt_delete(node_t* node) {
  if (node == NULL) return;

  bt_delete(node->left);
  bt_delete(node->right);
  free(node);
}

node_t* rbtree_grand_parent(node_t* node) {
  if (node == NULL) return NULL;
  if (node->parent == NULL) return NULL;

  return node->parent->parent;
}

node_t* rbtree_uncle(node_t* node) {
  node_t* grand_parent = rbtree_grand_parent(node);

  if (grand_parent == NULL) return NULL;

  if (grand_parent->left != node->parent) {
    return grand_parent->left;
  } else {
    return grand_parent->right;
  }
}

node_t* rbtree_sibling(node_t* node) {
  if (node->parent == NULL) return NULL;

  if (node == node->parent->left) {
    return node->parent->right;
  } else {
    return node->parent->left;
  }
}

void rbtree_insert1(rbtree* tree, node_t* node) {
  if (node->parent == NULL) {
    node->color = RBTREE_BLACK;
  } else {
    rbtree_insert2(tree, node);
  }
}

void rbtree_insert2(rbtree* tree, node_t* node) {
  if (node->parent->color == RBTREE_BLACK) {
    return;
  } else {
    rbtree_insert3(tree, node);
  }
}

void rbtree_insert3(rbtree* tree, node_t* node) {
  node_t* uncle = rbtree_uncle(node);

  if (uncle != NULL && uncle->color == RBTREE_RED) {
    node->parent->color = RBTREE_BLACK;
    uncle->color = RBTREE_BLACK;
    
    node_t* grand_parent = rbtree_grand_parent(node);
    grand_parent->color = RBTREE_RED;
    rbtree_insert1(tree, grand_parent);
  } else {
    rbtree_insert4(tree, node);
  }
}

void rbtree_insert4(rbtree* tree, node_t* node) {
  node_t* grand_parent = rbtree_grand_parent(node);

  if (node == node->parent->right && node->parent == grand_parent->left) {
    rotate_left(tree, node->parent);
    node = node->left;
  } else if (node == node->parent->left && node->parent == grand_parent->right) {
    rotate_right(tree, node->parent);
    node = node->right;
  }

  rbtree_insert5(tree, node);
}

void rbtree_insert5(rbtree* tree, node_t* node) {
  node_t* grand_parent = rbtree_grand_parent(node);

  node->parent->color = RBTREE_BLACK;
  grand_parent->color = RBTREE_RED;
  if (node == node->parent->left) {
    rotate_right(tree, grand_parent);
  } else {
    rotate_left(tree, grand_parent);
  }
}

void rotate_left(rbtree* tree, node_t* node) {
    node_t* child = node->right;
    node_t* parent = node->parent;

    node->right = child->left;
    if (child->left != NULL) {
        child->left->parent = node;
    }

    child->parent = parent;
    if (parent != NULL) {
        if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
    } else {
        tree->root = child;
    }

    child->left = node;
    node->parent = child;
}

void rotate_right(rbtree* tree, node_t* node) {
    node_t* child = node->left;
    node_t* parent = node->parent;

    node->left = child->right;
    if (child->right != NULL) {
        child->right->parent = node;
    }

    child->parent = parent;
    if (parent != NULL) {
        if (parent->right == node) {
            parent->right = child;
        } else {
            parent->left = child;
        }
    } else {
        tree->root = child;
    }

    child->right = node;
    node->parent = child;
}

int rbtree_is_leaf(node_t* node) {  
  return (node->left == NULL && node->right == NULL);
}

void rbtree_replace(node_t* node, node_t* child) {
  child->parent = node->parent;
  if (node->parent->left == node) {
    node->parent->left = child;
  } else if (node->parent->right == node) {
    node->parent->right = child;
  }
  // key_t temp_key = node->key;
  // node->key = child->key;
  // child->key = temp_key;
}

void delete_one_child(rbtree *tree, node_t *node) {
 if (tree->root == node) {
    // root 노드가 자식이 없으면 root를 NULL로 설정
    if (node->left == NULL && node->right == NULL) {
      tree->root = NULL;
    } else {
      // root 노드가 자식이 하나인 경우
      // 자식을 새로운 root로 설정하고 부모를 NULL로 설정
      if (node->left != NULL) {
        tree->root = node->left;
        node->left->parent = NULL;
      } else {
        tree->root = node->right;
        node->right->parent = NULL;
      }
    }
    // 노드를 해제합니다.
    free(node);
    return;
  }

  node_t* child;

  if(node->left == NULL) {
    child = node->right;
  } else {
    child = node->left;
  }

  rbtree_replace(node, child);

  // 삭제된 노드가 검정이면서 새로운 자식이 레드인 경우
  if (node->color == RBTREE_BLACK) {
    if (child != NULL && child->color == RBTREE_RED) {
      child->color = RBTREE_BLACK;
    } else {
      rbtree_delete1(tree, child);
    }
  }

  free(node);
}

void rbtree_delete1(rbtree* tree, node_t* node) {
  if (node->parent != NULL) {
    rbtree_delete2(tree, node);
  }
}

void rbtree_delete2(rbtree* tree, node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  if (sibling->color == RBTREE_RED) {
    node->parent->color = RBTREE_RED;
    sibling->color = RBTREE_BLACK;
    if (node == node->parent->left) {
      rotate_left(tree, node->parent);
    } else {
      rotate_right(tree, node->parent);
    }
  }
  
  rbtree_delete3(tree, node);
}

void rbtree_delete3(rbtree* tree, node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  if (
    node->parent->color == RBTREE_BLACK
    && sibling->color == RBTREE_BLACK
    && sibling->left->color == RBTREE_BLACK
    && sibling->right->color == RBTREE_BLACK
  ) {
    sibling->color = RBTREE_RED;
    rbtree_delete1(tree, node->parent);
  } else {
    rbtree_delete4(tree, node);
  }
}

void rbtree_delete4(rbtree* tree, node_t* node) {
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
    rbtree_delete5(tree, node);
  }
}

void rbtree_delete5(rbtree* tree, node_t* node) {
  node_t* sibling = rbtree_sibling(node);

 if (sibling->color == RBTREE_BLACK) {
    if (
      node == node->parent->left &&
      (sibling->right == NULL || sibling->right->color == RBTREE_BLACK) &&
      (sibling->left != NULL && sibling->left->color == RBTREE_RED)
    ) {
      sibling->color = RBTREE_RED;
      sibling->left->color = RBTREE_BLACK;
      rotate_right(tree, sibling);
    } else if (
      node == node->parent->right &&
      (sibling->left == NULL || sibling->left->color == RBTREE_BLACK) &&
      (sibling->right != NULL && sibling->right->color == RBTREE_RED)
    ) {
      sibling->color = RBTREE_RED;
      sibling->right->color = RBTREE_BLACK;
      rotate_left(tree, sibling);
    }
  }
  rbtree_delete6(tree, node);
}

void rbtree_delete6(rbtree* tree, node_t* node) {
  node_t* sibling = rbtree_sibling(node);

  sibling->color = node->parent->color;
  node->parent->color = RBTREE_BLACK;

  if (node == node->parent->left) {
    if (sibling->right != NULL) {
      sibling->right->color = RBTREE_BLACK;
    }
    rotate_left(tree, node->parent);
  } else {
    if (sibling->left != NULL) {
      sibling->left->color = RBTREE_BLACK;
    }
    rotate_right(tree, node->parent);
  }
}