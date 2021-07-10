#ifndef RBTREE_INCLUDE_LOG_H_
#define RBTREE_INCLUDE_LOG_H_


// source: https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#ifdef DEBUG
    #define debug_printf(fmt, ...) \
        do { printf("%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, __func__, __VA_ARGS__); } while(0)
    #define debug_print(msg) \
        do { printf("%s:%d:%s(): " msg "\n", __FILE__, __LINE__, __func__); } while(0)
#else   // DEBUG
    #define debug_printf(fmt, ...)
    #define debug_print(msg)
#endif  // DEBUG

#endif  // RBTREE_INCLUDE_LOG_H_
