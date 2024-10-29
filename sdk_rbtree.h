#ifndef INCLUDED_SDK_RBTREE_H
#define INCLUDED_SDK_RBTREE_H

#ifndef INCLUDED_SDK_TYPES_H
#include <sdk_types.h>
#endif /*INCLUDED_SDK_TYPES_H*/

#ifndef INCLUDED_SDK_ERRORS_H
#include <sdk_errors.h>
#endif /*INCLUDED_SDK_ERRORS_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

typedef int sdk_rbtree_key_t;

typedef enum sdk_rbtree_color_enum{
    kSDK_RBTREE_COLOR_RED = 0,
    kSDK_RBTREE_COLOR_BLACK = 1,
}sdk_rbtree_color_t;

typedef struct sdk_rbtree_node_s sdk_rbtree_node_t;

struct sdk_rbtree_node_s{
    sdk_rbtree_key_t key;
    void* value;

    sdk_rbtree_node_t *left;
    sdk_rbtree_node_t *right;
    sdk_rbtree_node_t *parent;

    sdk_rbtree_color_t color;
};

typedef struct sdk_rbtree_tree_s{
    sdk_rbtree_node_t * root;
    sdk_rbtree_node_t * nil;
}sdk_rbtree_tree_t;

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

sdk_err_t sdk_rbtree_insert(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * n);

sdk_rbtree_node_t * sdk_rbtree_delete(sdk_rbtree_tree_t * self, sdk_rbtree_node_t * n);

sdk_rbtree_node_t * sdk_rbtree_search(sdk_rbtree_tree_t * self, sdk_rbtree_key_t key);









#endif /*INCLUDED_SDK_RBTREE_H*/
