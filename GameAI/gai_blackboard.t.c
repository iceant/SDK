#include <gai_blackboard.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int random(int min, int max){
    return min + rand() % (max-min+1);
}

gai_knowledge_evaluate_result_t* Random_Evaluator(gai_knowledgesource_t * self, void* ud, gai_knowledge_evaluate_result_t* result){
    result->confidence = random(0, 10);
    result->evaluation = (void*)random(1, 100);
    return result;
}

int main(int argc, char** argv){
    srand(time(0));

    gai_blackboard_t b;
    gai_blackboard_init(&b, 0);

    gai_knowledgesource_t source1;
    gai_knowledgesource_init(&source1, Random_Evaluator, 1, 0);

    gai_knowledgesource_t source2;
    gai_knowledgesource_init(&source2, Random_Evaluator, 1, 0);

    gai_blackboard_add_source(&b, "value", &source1);
    gai_blackboard_add_source(&b, "value", &source2);

    for(int i=0; i<10; i++){
        int value = (int)gai_blackboard_get(&b, "value", 0);
        printf("[%d] %d\n", i, value);
    }

    gai_blackboard_set(&b, "age", (void*)random(1, 100));
    printf("age: %d\n", (int)gai_blackboard_get(&b,"age", 0));

    gai_blackboard_destroy(&b);

    return 0;
}