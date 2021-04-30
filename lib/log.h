#ifndef _RB_TREE_LOG_H_
#define _RB_TREE_LOG_H_


/* source: https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c */
#ifdef DEBUG
    #define debug_print(fmt, ...) \
        do { fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while(0)
#else   /* DEBUG */
    #define debug_print(fmt, ...)
#endif  /* DEBUG */

#endif  /* _RB_TREE_LOG_H_ */