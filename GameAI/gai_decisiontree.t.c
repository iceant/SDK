#include <gai_decisiontree.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


static bool stop;

int random(int min, int max){
    return min + rand() % (max-min+1);
}

static int Action_Initialize(gai_action_t * action, void* ud){
    printf("Action::%s Initialize\n", action->name);
    return 0;
}

static int Action_Update(gai_action_t * action, void* ud){
    int status = random(0, 1);
    printf("Action::%s Update -> %d\n", action->name, status);
    if(status==0){
        action->status = GAI_ACTION_STATUS_TERMINATED;
    }else{
        action->status = GAI_ACTION_STATUS_RUNNING;
    }
    return 0;
}

static int Action_Cleanup(gai_action_t * action, void* ud){
    printf("Action::%s Cleanup\n", action->name);
    return 0;
}

static int Action_Exit(gai_action_t* action, void* ud){
    printf("Action::%s Exit\n", action->name);
    stop = true;
    return 0;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

static int Random_Evaluator(gai_decisiontreenode_t* self, void* ud){
    int v =  random(0, 1);
    printf("Evaluator::%s -> %d\n", self->name, v);
    return v;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */



int main(int argc, char** argv){
    stop = false;
    srand(time(0));
    
    gai_decisiontree_t Soldier_DecisionTree;
    gai_decisiontree_init(&Soldier_DecisionTree);
    
    gai_decisiontreenode_t isAliveBranch;
    gai_decisiontreenode_t criticalBranch;
    gai_decisiontreenode_t moveFleeBranch;
    gai_decisiontreenode_t enemyBranch;
    gai_decisiontreenode_t ammoBranch;
    gai_decisiontreenode_t shootBranch;
    gai_decisiontreenode_t moveRandomBranch;
    gai_decisiontreenode_t randomBranch;
    
    gai_decisiontreenode_init(&isAliveBranch, "isAliveBranch");
    gai_decisiontreenode_init(&criticalBranch, "criticalBranc");
    gai_decisiontreenode_init(&moveFleeBranch, "moveFleeBranch");
    gai_decisiontreenode_init(&enemyBranch, "enemyBranch");
    gai_decisiontreenode_init(&ammoBranch, "ammoBranch");
    gai_decisiontreenode_init(&shootBranch, "shootBranch");
    gai_decisiontreenode_init(&moveRandomBranch, "moveRandomBranch");
    gai_decisiontreenode_init(&randomBranch, "randomBranch");
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* isAliveBranch */
    
    gai_decisiontreenode_addchild(&isAliveBranch, &criticalBranch);
    
    gai_decisiontreenode_t DieActionLeaf;
    gai_decisiontreenode_init(&DieActionLeaf, "DieActionLeaf");
    gai_action_t DieAction;
    gai_action_init(&DieAction, "DieAction", Action_Initialize, Action_Update, Action_Exit, 0);
    DieActionLeaf.action = &DieAction;
    gai_decisiontreenode_addchild(&isAliveBranch, &DieActionLeaf);
    
    gai_decisiontreenode_set_evaluator(&isAliveBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* criticalBranch */
    gai_decisiontreenode_addchild(&criticalBranch, &moveFleeBranch);
    gai_decisiontreenode_addchild(&criticalBranch, &enemyBranch);
    gai_decisiontreenode_set_evaluator(&criticalBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* moveFleeBranch */
    gai_decisiontreenode_t MoveActionLeaf;
    gai_decisiontreenode_init(&MoveActionLeaf, "MoveActionLeaf");
    gai_action_t MoveAction;
    gai_action_init(&MoveAction, "MoveAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    MoveActionLeaf.action = &MoveAction;
    gai_decisiontreenode_addchild(&moveFleeBranch, &MoveActionLeaf);
    
    gai_decisiontreenode_t FleeActionLeaf;
    gai_decisiontreenode_init(&FleeActionLeaf, "FleeActionLeaf");
    gai_action_t FleeAction;
    gai_action_init(&FleeAction, "FleeAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    FleeActionLeaf.action = &FleeAction;
    gai_decisiontreenode_addchild(&moveFleeBranch, &FleeActionLeaf);
    
    gai_decisiontreenode_set_evaluator(&moveFleeBranch, Random_Evaluator);
    
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* enemyBranch */
    
    gai_decisiontreenode_addchild(&enemyBranch, &ammoBranch);
    gai_decisiontreenode_addchild(&enemyBranch, &moveRandomBranch);
    gai_decisiontreenode_set_evaluator(&enemyBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* ammoBranch */
    
    gai_decisiontreenode_addchild(&ammoBranch, &shootBranch);
    
    gai_decisiontreenode_t ReloadActionLeaf;
    gai_decisiontreenode_init(&ReloadActionLeaf, "ReloadActionLeaf");
    gai_action_t ReloadAction;
    gai_action_init(&ReloadAction, "ReloadAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    ReloadActionLeaf.action = &ReloadAction;
    gai_decisiontreenode_addchild(&ammoBranch, &ReloadActionLeaf);
    
    gai_decisiontreenode_set_evaluator(&ammoBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* shootBranch */
    
    gai_decisiontreenode_t ShootActionLeaf;
    gai_decisiontreenode_init(&ShootActionLeaf, "ShootActionLeaf");
    gai_action_t ShootAction;
    gai_action_init(&ShootAction, "ShootAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    ShootActionLeaf.action = &ShootAction;
    gai_decisiontreenode_addchild(&shootBranch, &ShootActionLeaf);
    
    
    gai_decisiontreenode_t PursueActionLeaf;
    gai_decisiontreenode_init(&PursueActionLeaf, "PursueActionLeaf");
    gai_action_t PursueAction;
    gai_action_init(&PursueAction, "PursueAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    PursueActionLeaf.action = &PursueAction;
    gai_decisiontreenode_addchild(&shootBranch, &PursueActionLeaf);
    
    gai_decisiontreenode_set_evaluator(&shootBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* moveRandomBranch */
    gai_decisiontreenode_t MoveRandomActionLeaf;
    gai_decisiontreenode_init(&MoveRandomActionLeaf, "MoveRandomActionLeaf");
    gai_action_t MoveRandomAction;
    gai_action_init(&MoveRandomAction, "MoveRandomAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    MoveRandomActionLeaf.action = &MoveRandomAction;
    gai_decisiontreenode_addchild(&moveRandomBranch, &MoveRandomActionLeaf);
    gai_decisiontreenode_addchild(&moveRandomBranch, &randomBranch);
    
    gai_decisiontreenode_set_evaluator(&moveRandomBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /* randomBranch */
    gai_decisiontreenode_t RandomMoveActionLeaf;
    gai_decisiontreenode_init(&RandomMoveActionLeaf, "RandomMoveActionLeaf");
    gai_action_t RandomMoveAction;
    gai_action_init(&RandomMoveAction, "RandomMoveAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    RandomMoveActionLeaf.action = &RandomMoveAction;
    gai_decisiontreenode_addchild(&randomBranch, &RandomMoveActionLeaf);
    
    
    gai_decisiontreenode_t IdleActionLeaf;
    gai_decisiontreenode_init(&IdleActionLeaf, "IdleActionLeaf");
    gai_action_t IdleAction;
    gai_action_init(&IdleAction, "IdleAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    IdleActionLeaf.action = &IdleAction;
    gai_decisiontreenode_addchild(&randomBranch, &IdleActionLeaf);
    
    gai_decisiontreenode_set_evaluator(&randomBranch, Random_Evaluator);
    
    /* -------------------------------------------------------------------------------------------------------------- */
    /*  */
    
    
    gai_decisiontree_set_branch(&Soldier_DecisionTree, &isAliveBranch);
    
    while(!stop){
        gai_decisiontree_update(&Soldier_DecisionTree, 0);
    }
    
    gai_decisiontreenode_destroy(&DieActionLeaf);
    gai_decisiontreenode_destroy(&MoveActionLeaf);
    gai_decisiontreenode_destroy(&FleeActionLeaf);
    gai_decisiontreenode_destroy(&IdleActionLeaf);
    gai_decisiontreenode_destroy(&MoveRandomActionLeaf);
    gai_decisiontreenode_destroy(&PursueActionLeaf);
    gai_decisiontreenode_destroy(&RandomMoveActionLeaf);
    gai_decisiontreenode_destroy(&ReloadActionLeaf);
    gai_decisiontreenode_destroy(&ShootActionLeaf);
    
    gai_decisiontreenode_destroy(&isAliveBranch);
    gai_decisiontreenode_destroy(&criticalBranch);
    gai_decisiontreenode_destroy(&moveFleeBranch);
    gai_decisiontreenode_destroy(&enemyBranch);
    gai_decisiontreenode_destroy(&ammoBranch);
    gai_decisiontreenode_destroy(&shootBranch);
    gai_decisiontreenode_destroy(&moveRandomBranch);
    gai_decisiontreenode_destroy(&randomBranch);
    gai_decisiontree_destroy(&Soldier_DecisionTree);
    
    
    return 0;
}