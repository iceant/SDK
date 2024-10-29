#include <sdk_rbtree.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_memory.h>

static int KeyArray[20] = { 24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15 };

static void preorder(sdk_rbtree_tree_t* tree, sdk_rbtree_node_t* node)
{
    if(node!= tree->nil)
    {
        printf("key:%d, color:%d\n", node->key, node->color);
        preorder(tree, node->left);
        preorder(tree, node->right);
    }
}

static void inorder(sdk_rbtree_tree_t* tree, sdk_rbtree_node_t* node)
{
    if(node!= tree->nil)
    {
        inorder(tree, node->left);
        printf("key:%d, color:%d\n", node->key, node->color);
        inorder(tree, node->right);
    }
}

static void postorder(sdk_rbtree_tree_t* tree, sdk_rbtree_node_t* node)
{
    if(node != tree->nil)
    {
        postorder(tree, node->left);
        postorder(tree, node->right);
        printf("key:%d, color:%d\n", node->key, node->color);
    }
}

static void rbtree_traversal(sdk_rbtree_tree_t *T,
                             sdk_rbtree_node_t *node) {
//    if (node != T->nil) {
//        rbtree_traversal(T, node->left);
//        printf("key:%d, color:%d\n", node->key, node->color);
//        rbtree_traversal(T, node->right);
//    }

//    preorder(T, node);
//    inorder(T, node);
    postorder(T, node);
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