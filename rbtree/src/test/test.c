#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct Test {
    char name[20];
    char error_msg[100];
    char succ_msg[100];
    bool (*test_action)();
}; 

bool height_test()
{
    return true;
}

bool search_test()
{
    return true;
}

bool insert_test()
{
    return true;
}

bool delete_test()
{
    return true;
}

#define TEST_COUNT (sizeof(tests) / sizeof(struct Test))
static struct Test tests[] = {
    {"height", "Height test failed!", "Height test success!", &height_test},
    {"search", "Search test failed!", "Search test success!", &search_test},
    {"height", "Insert test failed!", "Insert test success!", &insert_test},
    {"height", "Delete test failed!", "Delete test success!", &delete_test},
};

void test(const char* test_name)
{
    struct Test* current = NULL;

    for (size_t i = 0; i < TEST_COUNT; i++)
        if (strcmp(test_name, tests[i].name) == 0) current = tests + i;
    
    bool res = current->test_action();

    if (res) printf("SUCCESS: %s\n", current->succ_msg);
    else printf("FAILURE: " "%s\n", current->error_msg);
}

void test_all()
{
    for (size_t i = 0; i < sizeof(tests) / sizeof(struct Test); i++) {
        struct Test current = tests[i];
        bool res = current.test_action();

        if (res) printf("SUCCESS: %s\n", current.succ_msg);
        else printf("FAILURE: " "%s\n", current.error_msg);
    }
}

void main(int argc, char** argv)
{
    if (argc < 2) {
        test_all();
    } else {
        char* name = argv[1];
        test(name);
    }
}