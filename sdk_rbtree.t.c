#include <sdk_rbtree.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_memory.h>

static int KeyArray[20] = { 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15 };

static void rbtree_traversal(sdk_rbtree_tree_t *T,
                             sdk_rbtree_node_t *node) {
    if (node != T->nil) {
        rbtree_traversal(T, node->left);
        printf("key:%d, color:%d\n", node->key, node->color);
        rbtree_traversal(T, node->right);
    }
}

int main(int argc, char** argv){

    sdk_rbtree_tree_t tree;
    tree.nil = SDK_ALLOC(sizeof(*tree.nil));
    tree.nil->color = kSDK_RBTREE_COLOR_BLACK;
    tree.root = tree.nil;

    sdk_rbtree_node_t * node = tree.nil;
    for(int i=0; i<20; i++){
        node = SDK_ALLOC(sizeof(*node));
        node->key = KeyArray[i];
        node->value = 0;
        printf("insert arr[%d]=%d\n", i, node->key);
        sdk_rbtree_insert(&tree, node);
        printf("insert end\n");
    }

    printf("----------------------------------------\n");
    rbtree_traversal(&tree, tree.root);
    printf("----------------------------------------\n");

    for (int i = 0; i < 20; i++) {
        printf("search key = %d\n", KeyArray[i]);
        sdk_rbtree_node_t *node = sdk_rbtree_search(&tree, KeyArray[i]);

        if(node!=tree.nil)
        {
            printf("delete key = %d ", node->key);
            sdk_rbtree_node_t *cur = sdk_rbtree_delete(&tree, node);
            SDK_FREE(cur);
            printf(" done!\n");
        }
        else
            break;

        printf("show rbtree: \n");
        rbtree_traversal(&tree, tree.root);
        printf("----------------------------------------\n");
    }


    return 0;
}