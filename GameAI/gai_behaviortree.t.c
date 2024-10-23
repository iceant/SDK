#include "gai_behaviortree.h"
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

static bool Random_Evaluator(gai_evaluator_t * self, void* ud){
    int v =  random(0, 1);
    printf("Evaluator::%s -> %d\n", self->name, v);
    return v;
}

int main(int argc, char** argv){
    srand(time(0));
    stop = false;
    gai_behaviortree_t tree;
    gai_behaviortree_init(&tree, 0);

    gai_behaviortreenode_t * node;

    gai_behaviortreenode_t root;
    gai_behaviortree_selector_node_init(&root, "root");
    gai_behaviortree_set_root(&tree, &root);
    node = &root;

    /* -------------------------------------------------------------------------------------------------------------- */
    /* die action */

    gai_behaviortreenode_t child;
    gai_behaviortree_sequence_node_init(&child, "sequence");
    gai_behaviortreenode_add_child(node, &child);
    node = &child;

    gai_behaviortreenode_t IsNotAliveCondition;
    gai_evaluator_t IsNotAliveEvaluator;
    gai_evaluator_init(&IsNotAliveEvaluator, "IsNotAliveEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&IsNotAliveCondition, "is not alive", &IsNotAliveEvaluator);
    gai_behaviortreenode_add_child(node, &IsNotAliveCondition);

    gai_behaviortreenode_t DieActionNode;
    gai_action_t DieAction;
    gai_action_init(&DieAction, "DieAction", Action_Initialize, Action_Update, Action_Exit, 0);
    gai_behaviortree_action_node_init(&DieActionNode, "die", &DieAction);
    gai_behaviortreenode_add_child(node, &DieActionNode);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* flee action */

    gai_behaviortreenode_t FleeSequenceNode;
    gai_behaviortree_sequence_node_init(&FleeSequenceNode, "FleeSequence");
    gai_behaviortreenode_add_child(&root, &FleeSequenceNode);
    node = &FleeSequenceNode;

    gai_behaviortreenode_t HasCritialHealthCondition;
    gai_evaluator_t HasCritialHealthEvaluator;
    gai_evaluator_init(&HasCritialHealthEvaluator, "HasCritialHealthEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&HasCritialHealthCondition, "has critical health", &HasCritialHealthEvaluator);
    gai_behaviortreenode_add_child(node, &HasCritialHealthCondition);


    gai_behaviortreenode_t FleeActionNode;
    gai_action_t FleeAction;
    gai_action_init(&FleeAction, "FleeAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&FleeActionNode, "flee", &FleeAction);
    gai_behaviortreenode_add_child(node, &FleeActionNode);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* reload/shoot/pursue actions */
    gai_behaviortreenode_t ReloadShootPursueSequenceNode;
    gai_behaviortree_sequence_node_init(&ReloadShootPursueSequenceNode, "ReloadShootPursueSequence");
    gai_behaviortreenode_add_child(&root, &ReloadShootPursueSequenceNode);
    node = &ReloadShootPursueSequenceNode;

    gai_behaviortreenode_t HasEnemyCondition;
    gai_evaluator_t HasEnemyEvaluator;
    gai_evaluator_init(&HasEnemyEvaluator, "HasEnemyEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&HasEnemyCondition, "has enemy", &HasEnemyEvaluator);
    gai_behaviortreenode_add_child(node, &HasEnemyCondition);

    gai_behaviortreenode_t ReloadShootPursueSelectorNode;
    gai_behaviortree_selector_node_init(&ReloadShootPursueSelectorNode, "ReloadShootPursueSelector");
    gai_behaviortreenode_add_child(node, &ReloadShootPursueSelectorNode);
    node = &ReloadShootPursueSelectorNode;


    gai_behaviortreenode_t ReloadSequenceNode;
    gai_behaviortree_sequence_node_init(&ReloadSequenceNode, "ReloadSequence");
    gai_behaviortreenode_add_child(node, &ReloadSequenceNode);
    node = &ReloadSequenceNode;

    gai_behaviortreenode_t HasNoAmmoCondition;
    gai_evaluator_t HasNoAmmoEvaluator;
    gai_evaluator_init(&HasNoAmmoEvaluator, "HasNoAmmoEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&HasNoAmmoCondition, "has no ammo", &HasNoAmmoEvaluator);
    gai_behaviortreenode_add_child(node, &HasNoAmmoCondition);


    gai_behaviortreenode_t ReloadActionNode;
    gai_action_t ReloadAction;
    gai_action_init(&ReloadAction, "ReloadAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&ReloadActionNode, "reload", &ReloadAction);
    gai_behaviortreenode_add_child(node, &ReloadActionNode);

    node = node->parent;

    gai_behaviortreenode_t ShootSequenceNode;
    gai_behaviortree_sequence_node_init(&ShootSequenceNode, "ShootSequence");
    gai_behaviortreenode_add_child(node, &ShootSequenceNode);
    node = &ShootSequenceNode;

    gai_behaviortreenode_t CanShootEnemyCondition;
    gai_evaluator_t CanShootEnemyEvaluator;
    gai_evaluator_init(&CanShootEnemyEvaluator, "CanShootEnemyEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&CanShootEnemyCondition, "can shoot enemy", &CanShootEnemyEvaluator);
    gai_behaviortreenode_add_child(node, &CanShootEnemyCondition);

    gai_behaviortreenode_t ShootActionNode;
    gai_action_t ShootAction;
    gai_action_init(&ShootAction, "ShootAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&ShootActionNode, "shoot", &ShootAction);
    gai_behaviortreenode_add_child(node, &ShootActionNode);

    node = node->parent;

    gai_behaviortreenode_t PursueActionNode;
    gai_action_t PursueAction;
    gai_action_init(&PursueAction, "PursueAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&PursueActionNode, "pursue", &PursueAction);
    gai_behaviortreenode_add_child(node, &PursueActionNode);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* move to position */


    node = &root;

    gai_behaviortreenode_t MoveToPositionSequenceNode;
    gai_behaviortree_sequence_node_init(&MoveToPositionSequenceNode, "MoveToPositionSequence");
    gai_behaviortreenode_add_child(node, &MoveToPositionSequenceNode);
    node = &MoveToPositionSequenceNode;

    gai_behaviortreenode_t HasMovePositionCondition;
    gai_evaluator_t HasMovePositionEvaluator;
    gai_evaluator_init(&HasMovePositionEvaluator, "HasMovePositionEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&HasMovePositionCondition, "has move position", &HasMovePositionEvaluator);
    gai_behaviortreenode_add_child(node, &HasMovePositionCondition);

    gai_behaviortreenode_t MoveToPositionActionNode;
    gai_action_t MoveToPositionAction;
    gai_action_init(&MoveToPositionAction, "MoveToPositionAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&MoveToPositionActionNode, "move to position", &MoveToPositionAction);
    gai_behaviortreenode_add_child(node, &MoveToPositionActionNode);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* random move */

    node = &root;
    gai_behaviortreenode_t RandomMoveSequenceNode;
    gai_behaviortree_sequence_node_init(&RandomMoveSequenceNode, "RandomMoveSequence");
    gai_behaviortreenode_add_child(node, &RandomMoveSequenceNode);
    node = &RandomMoveSequenceNode;

    gai_behaviortreenode_t HalfChanceCondition;
    gai_evaluator_t HalfChanceEvaluator;
    gai_evaluator_init(&HalfChanceEvaluator, "HalfChanceEvaluator", Random_Evaluator, 0);
    gai_behaviortree_condition_node_init(&HalfChanceCondition, "50/50 chance", &HalfChanceEvaluator);
    gai_behaviortreenode_add_child(node, &HalfChanceCondition);

    gai_behaviortreenode_t RandomMoveActionNode;
    gai_action_t RandomMoveAction;
    gai_action_init(&RandomMoveAction, "RandomMoveAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&RandomMoveActionNode, "random move", &RandomMoveAction);
    gai_behaviortreenode_add_child(node, &RandomMoveActionNode);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* idle */

    gai_behaviortreenode_t IdleActionNode;
    gai_action_t IdleAction;
    gai_action_init(&IdleAction, "IdleAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_behaviortree_action_node_init(&IdleActionNode, "idle", &IdleAction);
    gai_behaviortreenode_add_child(&root, &IdleActionNode);


    /* -------------------------------------------------------------------------------------------------------------- */
    /*  */


    while(!stop){
        gai_behaviortree_update(&tree, 0);
    }


    gai_behaviortree_destroy(&tree);
    return 0;
}