#include <sdk_rbtree.h>

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static void sdk_rbtree_left_rotate(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * x){
    sdk_rbtree_node_t * y = x->right;

    x->right = y->left;
    if(y->left != self->nil){
        y->left->parent= x;
    }

    y->parent = x->parent;
    if(x->parent == self->nil){
        self->root = y;
    }else if(x == x->parent->left){
        x->parent->left = y;
    }else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

static void sdk_rbtree_right_rotate(sdk_rbtree_tree_t *self, sdk_rbtree_node_t * y){
    sdk_rbtree_node_t * x = y->left;

    y->left = x->right;
    if(x->right != self->nil){
        x->right->parent = y;
    }

    x->parent = y->parent;
    if(y->parent == self->nil){
        self->root = x;
    }else if(y==y->parent->right){
        y->parent->right = x;
    }else{
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

static void sdk_rbtree_insert_fixup(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * z){
    while(z->parent->color == kSDK_RBTREE_COLOR_RED){
        if(z->parent == z->parent->parent->left){
            sdk_rbtree_node_t * y = z->parent->parent->right;
            if(y->color == kSDK_RBTREE_COLOR_RED){
                y->color = kSDK_RBTREE_COLOR_BLACK;
                z->parent->color = kSDK_RBTREE_COLOR_BLACK;
                z->parent->parent->color = kSDK_RBTREE_COLOR_RED;

                z = z->parent->parent;
            }
            else{
                if(z->parent->right==z){
                    z= z->parent;
                    sdk_rbtree_left_rotate(self, z);
                }

                z->parent->color = kSDK_RBTREE_COLOR_BLACK;
                z->parent->parent->color = kSDK_RBTREE_COLOR_RED;
                sdk_rbtree_right_rotate(self, z->parent->parent);
            }
        }else{
            sdk_rbtree_node_t * y = z->parent->parent->left;
            if(y->color == kSDK_RBTREE_COLOR_RED){
                z->parent->parent->color = kSDK_RBTREE_COLOR_RED;
                z->parent->color = kSDK_RBTREE_COLOR_BLACK;
                y->color = kSDK_RBTREE_COLOR_BLACK;

                z = z->parent->parent;
            }else{
                if(z==z->parent->left){
                    z = z->parent;
                    sdk_rbtree_right_rotate(self, z);
                }
                z->parent->color = kSDK_RBTREE_COLOR_BLACK;
                z->parent->parent->color =kSDK_RBTREE_COLOR_RED;
                sdk_rbtree_left_rotate(self, z->parent->parent);
            }
        }
    }
    self->root->color = kSDK_RBTREE_COLOR_BLACK;
}


static sdk_rbtree_node_t * sdk_rbtree_min(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * x){
    while(x->left != self->nil){
        x = x->left;
    }
    return x;
}

static sdk_rbtree_node_t * sdk_rbtree_max(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * x){
    while(x->right != self->nil){
        x = x->right;
    }
    return x;
}

static sdk_rbtree_node_t * sdk_rbtree_successor(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * x){
    sdk_rbtree_node_t * y = x->parent;
    if(x->right != self->nil){
        return sdk_rbtree_min(self, x->right);
    }
    while((y!=self->nil) && (x==y->right)){
        x = y;
        y = y->parent;
    }
    return y;
}
/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_err_t sdk_rbtree_insert(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * z)
{
    sdk_rbtree_node_t * pre = self->nil;
    sdk_rbtree_node_t * cur = self->root;

    while(cur!=self->nil){
        pre = cur;
        if(z->key > cur->key){
            cur = cur->right;
        }else if(z->key < cur->key){
            cur = cur->left;
        }else{
            return SDK_ERR_OK;
        }
    }

    z->parent = pre;
    if(pre == self->nil){
        self->root = z;
    }else{
        if(pre->key > z->key){
            pre->left = z;
        }else{
            pre->right = z;
        }
    }
    z->left = self->nil;
    z->right = self->nil;

    z->color = kSDK_RBTREE_COLOR_RED;

    sdk_rbtree_insert_fixup(self, z);
    return SDK_ERR_OK;
}


static void sdk_rbtree_delete_fixup(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * x){
    while((x!=self->root) && (x->color == kSDK_RBTREE_COLOR_BLACK)){
        if(x==x->parent->left){
            sdk_rbtree_node_t * w = x->parent->right;
            if(w->color == kSDK_RBTREE_COLOR_RED){
                w->color = kSDK_RBTREE_COLOR_BLACK;
                w->parent->color = kSDK_RBTREE_COLOR_RED;

                sdk_rbtree_left_rotate(self, x->parent);
                w = x->parent->right;
            }

            if((w->left->color==kSDK_RBTREE_COLOR_BLACK) && (w->right->color==kSDK_RBTREE_COLOR_BLACK)){
                w->color = kSDK_RBTREE_COLOR_RED;
                x = x->parent;
            }else{
                if(w->right->color == kSDK_RBTREE_COLOR_BLACK){
                    w->left->color = kSDK_RBTREE_COLOR_BLACK;
                    w->color = kSDK_RBTREE_COLOR_RED;
                    sdk_rbtree_right_rotate(self, w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = kSDK_RBTREE_COLOR_BLACK;
                w->right->color = kSDK_RBTREE_COLOR_BLACK;
                sdk_rbtree_left_rotate(self, x->parent);

                x = self->root;
            }
        }else{
            sdk_rbtree_node_t * w = x->parent->left;
            if(w->color == kSDK_RBTREE_COLOR_RED){
                w->color = kSDK_RBTREE_COLOR_BLACK;
                x->parent->color = kSDK_RBTREE_COLOR_RED;
                sdk_rbtree_right_rotate(self, x->parent);
                w = x->parent->left;
            }

            if((w->left->color == kSDK_RBTREE_COLOR_BLACK) && (w->right->color == kSDK_RBTREE_COLOR_BLACK)){
                w->color = kSDK_RBTREE_COLOR_RED;
                x = x->parent;
            }else{
                if(w->left->color == kSDK_RBTREE_COLOR_BLACK){
                    w->right->color = kSDK_RBTREE_COLOR_BLACK;
                    w->color = kSDK_RBTREE_COLOR_RED;
                    sdk_rbtree_left_rotate(self, w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = kSDK_RBTREE_COLOR_BLACK;
                w->left->color = kSDK_RBTREE_COLOR_BLACK;
                sdk_rbtree_right_rotate(self, x->parent);

                x = self->root;
            }
        }
    }
    x->color = kSDK_RBTREE_COLOR_BLACK;
}

sdk_rbtree_node_t * sdk_rbtree_delete(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * z){
    sdk_rbtree_node_t * y = self->nil;
    sdk_rbtree_node_t * x = self->nil;

    if((z->left == self->nil) || (z->right == self->nil)){
        y = z;
    }else{
        y = sdk_rbtree_successor(self, z);
    }

    if(y->left != self->nil){
        x = y->left;
    }else if(y->right!=self->nil){
        x = y->right;
    }
    x->parent = y->parent;
    if(y->parent == self->nil){
        self->root = x;
    }else if(y == y->parent->left){
        y->parent->left = x;
    }else{
        y->parent->right = x;
    }

    if(y!=z){
        z->key = y->key;
        z->value = y->value;
    }

    if(y->color == kSDK_RBTREE_COLOR_BLACK){
        sdk_rbtree_delete_fixup(self, x);
    }

    return y;
}

sdk_rbtree_node_t * sdk_rbtree_search(sdk_rbtree_tree_t * self, sdk_rbtree_key_t key)
{
    sdk_rbtree_node_t *node = self->root;
    while(node!=self->nil){
        if(key < node->key){
            node = node->left;
        }else if(key > node->key){
            node = node->right;
        }else{
            return node;
        }
    }
    return self->nil;
}

