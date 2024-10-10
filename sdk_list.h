#ifndef INCLUDED_SDK_LIST_H
#define INCLUDED_SDK_LIST_H

////////////////////////////////////////////////////////////////////////////////
////

typedef struct sdk_list_node_s sdk_list_node_t;

struct sdk_list_node_s{
    sdk_list_node_t * next;
    sdk_list_node_t * prev;
};

typedef sdk_list_node_t sdk_list_t;

////////////////////////////////////////////////////////////////////////////////
////
#define SDK_LIST_NEXT(N)        (N)->next
#define SDK_LIST_PREV(N)        (N)->prev
#define SDK_LIST_NEXT_PREV(N)   SDK_LIST_PREV(SDK_LIST_NEXT(N))
#define SDK_LIST_PREV_NEXT(N)   SDK_LIST_NEXT(SDK_LIST_PREV(N))

#define SDK_LIST_INIT(N) (SDK_LIST_NEXT(N) = SDK_LIST_PREV(N) = (N))

#define SDK_LIST_IS_EMPTY(N) (SDK_LIST_NEXT(N)==(N))

/*
 Lp -- L -- N -- Ln
 */
#define SDK_LIST_INSERT_AFTER(L, N) \
do{                                 \
    SDK_LIST_NEXT_PREV(L) = (N);    \
    SDK_LIST_NEXT(N) = SDK_LIST_NEXT(L); \
    SDK_LIST_NEXT(L) = (N);         \
    SDK_LIST_PREV(N) = (L);         \
}while(0)


/*
 Lp -- N -- L -- Ln
 */
#define SDK_LIST_INSERT_BEFORE(L, N) \
do{                                 \
    SDK_LIST_PREV_NEXT(L) = (N);    \
    SDK_LIST_PREV(N) = SDK_LIST_PREV(L); \
    SDK_LIST_NEXT(N) = (L);         \
    SDK_LIST_PREV(L) = (N);         \
}while(0)


#define SDK_LIST_REMOVE(N) \
do{                        \
    SDK_LIST_PREV_NEXT(N) = SDK_LIST_NEXT(N); \
    SDK_LIST_NEXT_PREV(N) = SDK_LIST_PREV(N); \
    SDK_LIST_INIT(N);      \
}while(0)


#endif /*INCLUDED_SDK_LIST_H*/
