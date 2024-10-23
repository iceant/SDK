#include <gai_fsm.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static bool stop = false;

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
    return v==0?false:true;
}

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

int main(int argc, char** argv){

    stop = false;
    srand(time(0));

    gai_fsm_t fsm;
    gai_fsm_init(&fsm, 0);

    gai_fsm_state_t DieState;
    gai_action_t DieAction;
    gai_action_init(&DieAction, "DieAction", Action_Initialize, Action_Update, Action_Exit, 0);
    gai_fsm_state_init(&DieState, "die", &DieAction);
    gai_fsm_add_state(&fsm, &DieState);

    gai_fsm_state_t FleeState;
    gai_action_t FleeAction;
    gai_action_init(&FleeAction, "FleeAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_fsm_state_init(&FleeState, "flee", &FleeAction);
    gai_fsm_add_state(&fsm, &FleeState);

    gai_fsm_state_t IdleState;
    gai_action_t IdleAction;
    gai_action_init(&IdleAction, "IdleAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_fsm_state_init(&IdleState, "idle", &IdleAction);
    gai_fsm_add_state(&fsm, &IdleState);

    gai_fsm_state_t MoveState;
    gai_action_t MoveAction;
    gai_action_init(&MoveAction, "MoveAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_fsm_state_init(&MoveState, "move", &MoveAction);
    gai_fsm_add_state(&fsm, &MoveState);

    gai_fsm_state_t PursueState;
    gai_action_t PursueAction;
    gai_action_init(&PursueAction, "PursueAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_fsm_state_init(&PursueState, "pursue", &PursueAction);
    gai_fsm_add_state(&fsm, &PursueState);

    gai_fsm_state_t RandomMoveState;
    gai_action_t RandomMoveAction;
    gai_action_init(&RandomMoveAction, "RandomMoveAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_fsm_state_init(&RandomMoveState, "randomMove", &RandomMoveAction);
    gai_fsm_add_state(&fsm, &RandomMoveState);

    gai_fsm_state_t ReloadState;
    gai_action_t ReloadAction;
    gai_action_init(&ReloadAction, "ReloadAction", Action_Initialize, Action_Update, Action_Cleanup, 0);
    gai_fsm_state_init(&ReloadState, "reload", &ReloadAction);
    gai_fsm_add_state(&fsm, &ReloadState);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* idle action */
    gai_fsm_transition_t IdleToDieTransition;
    gai_evaluator_t IdleToDieEvaluator;
    gai_evaluator_init(&IdleToDieEvaluator, "IdleToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToDieTransition, "die", &IdleToDieEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToDieTransition);

    gai_fsm_transition_t IdleToFleeTransition;
    gai_evaluator_t IdleToFleeEvaluator;
    gai_evaluator_init(&IdleToFleeEvaluator, "IdleToFlee", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToFleeTransition, "flee", &IdleToFleeEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToFleeTransition);

    gai_fsm_transition_t IdleToReloadTransition;
    gai_evaluator_t IdleToReloadEvaluator;
    gai_evaluator_init(&IdleToReloadEvaluator, "IdleToReload", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToReloadTransition, "reload", &IdleToReloadEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToReloadTransition);

    gai_fsm_transition_t IdleToShootTransition;
    gai_evaluator_t IdleToShootEvaluator;
    gai_evaluator_init(&IdleToShootEvaluator, "IdleToShoot", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToShootTransition, "shoot", &IdleToShootEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToShootTransition);


    gai_fsm_transition_t IdleToPursueTransition;
    gai_evaluator_t IdleToPursueEvaluator;
    gai_evaluator_init(&IdleToPursueEvaluator, "IdleToPursue", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToPursueTransition, "pursue", &IdleToPursueEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToPursueTransition);

    gai_fsm_transition_t IdleToRandomMoveTransition;
    gai_evaluator_t IdleToRandomMoveEvaluator;
    gai_evaluator_init(&IdleToRandomMoveEvaluator, "IdleToRandomMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToRandomMoveTransition, "randomMove", &IdleToRandomMoveEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToRandomMoveTransition);

    gai_fsm_transition_t IdleToIdleTransition;
    gai_evaluator_t IdleToIdleEvaluator;
    gai_evaluator_init(&IdleToIdleEvaluator, "IdleToIdle", Random_Evaluator, 0);
    gai_fsm_transition_init(&IdleToIdleTransition, "idle", &IdleToIdleEvaluator);
    gai_fsm_add_transition(&fsm, "idle", &IdleToIdleTransition);


    /* -------------------------------------------------------------------------------------------------------------- */
    /* move action */
    gai_fsm_transition_t MoveToDieTransition;
    gai_evaluator_t MoveToDieEvaluator;
    gai_evaluator_init(&MoveToDieEvaluator, "MoveToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToDieTransition, "die", &MoveToDieEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToDieTransition);

    gai_fsm_transition_t MoveToFleeTransition;
    gai_evaluator_t MoveToFleeEvaluator;
    gai_evaluator_init(&MoveToFleeEvaluator, "MoveToFlee", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToFleeTransition, "flee", &MoveToFleeEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToFleeTransition);


    gai_fsm_transition_t MoveToReloadTransition;
    gai_evaluator_t MoveToReloadEvaluator;
    gai_evaluator_init(&MoveToReloadEvaluator, "MoveToReload", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToReloadTransition, "reload", &MoveToReloadEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToReloadTransition);

    gai_fsm_transition_t MoveToShootTransition;
    gai_evaluator_t MoveToShootEvaluator;
    gai_evaluator_init(&MoveToShootEvaluator, "MoveToShoot", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToShootTransition, "shoot", &MoveToShootEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToShootTransition);

    gai_fsm_transition_t MoveToPursueTransition;
    gai_evaluator_t MoveToPursueEvaluator;
    gai_evaluator_init(&MoveToPursueEvaluator, "MoveToPursue", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToPursueTransition, "pursue", &MoveToPursueEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToPursueTransition);

    gai_fsm_transition_t MoveToMoveTransition;
    gai_evaluator_t MoveToMoveEvaluator;
    gai_evaluator_init(&MoveToMoveEvaluator, "MoveToMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToMoveTransition, "move", &MoveToMoveEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToMoveTransition);

    gai_fsm_transition_t MoveToRandomMoveTransition;
    gai_evaluator_t MoveToRandomMoveEvaluator;
    gai_evaluator_init(&MoveToRandomMoveEvaluator, "MoveToRandomMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&MoveToRandomMoveTransition, "randomMove", &MoveToRandomMoveEvaluator);
    gai_fsm_add_transition(&fsm, "move", &MoveToRandomMoveTransition);


    /* -------------------------------------------------------------------------------------------------------------- */
    /* randomMove */
    gai_fsm_transition_t RandomMoveToDieTransition;
    gai_evaluator_t RandomMoveToDieEvaluator;
    gai_evaluator_init(&RandomMoveToDieEvaluator, "RandomMoveToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&RandomMoveToDieTransition, "die", &RandomMoveToDieEvaluator);
    gai_fsm_add_transition(&fsm, "randomMove", &RandomMoveToDieTransition);

    gai_fsm_transition_t RandomMoveToMoveTransition;
    gai_evaluator_t RandomMoveToMoveEvaluator;
    gai_evaluator_init(&RandomMoveToMoveEvaluator, "RandomMoveToMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&RandomMoveToMoveTransition, "move", &RandomMoveToMoveEvaluator);
    gai_fsm_add_transition(&fsm, "randomMove", &RandomMoveToMoveTransition);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* shoot */

    gai_fsm_transition_t ShootToDieTransition;
    gai_evaluator_t ShootToDieEvaluator;
    gai_evaluator_init(&ShootToDieEvaluator, "ShootToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToDieTransition, "die", &ShootToDieEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToDieTransition);


    gai_fsm_transition_t ShootToFleeTransition;
    gai_evaluator_t ShootToFleeEvaluator;
    gai_evaluator_init(&ShootToFleeEvaluator, "ShootToFlee", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToFleeTransition, "flee", &ShootToFleeEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToFleeTransition);


    gai_fsm_transition_t ShootToReloadTransition;
    gai_evaluator_t ShootToReloadEvaluator;
    gai_evaluator_init(&ShootToReloadEvaluator, "ShootToReload", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToReloadTransition, "reload", &ShootToReloadEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToReloadTransition);

    gai_fsm_transition_t ShootToShootTransition;
    gai_evaluator_t ShootToShootEvaluator;
    gai_evaluator_init(&ShootToShootEvaluator, "ShootToShoot", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToShootTransition, "shoot", &ShootToShootEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToShootTransition);


    gai_fsm_transition_t ShootToPursueTransition;
    gai_evaluator_t ShootToPursueEvaluator;
    gai_evaluator_init(&ShootToPursueEvaluator, "ShootToPursue", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToPursueTransition, "pursue", &ShootToPursueEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToPursueTransition);

    gai_fsm_transition_t ShootToRandomMoveTransition;
    gai_evaluator_t ShootToRandomMoveEvaluator;
    gai_evaluator_init(&ShootToRandomMoveEvaluator, "ShootToRandomMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToRandomMoveTransition, "randomMove", &ShootToRandomMoveEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToRandomMoveTransition);

    gai_fsm_transition_t ShootToIdleTransition;
    gai_evaluator_t ShootToIdleEvaluator;
    gai_evaluator_init(&ShootToIdleEvaluator, "ShootToIdle", Random_Evaluator, 0);
    gai_fsm_transition_init(&ShootToIdleTransition, "idle", &ShootToIdleEvaluator);
    gai_fsm_add_transition(&fsm, "shoot", &ShootToIdleTransition);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* flee */
    gai_fsm_transition_t FleeToDieTransition;
    gai_evaluator_t FleeToDieEvaluator;
    gai_evaluator_init(&FleeToDieEvaluator, "FleeToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&FleeToDieTransition, "die", &FleeToDieEvaluator);
    gai_fsm_add_transition(&fsm, "flee", &FleeToDieTransition);

    gai_fsm_transition_t FleeToMoveTransition;
    gai_evaluator_t FleeToMoveEvaluator;
    gai_evaluator_init(&FleeToMoveEvaluator, "FleeToMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&FleeToMoveTransition, "move", &FleeToMoveEvaluator);
    gai_fsm_add_transition(&fsm, "flee", &FleeToMoveTransition);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* pursue */

    gai_fsm_transition_t PursueToDieTransition;
    gai_evaluator_t PursueToDieEvaluator;
    gai_evaluator_init(&PursueToDieEvaluator, "PursueToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&PursueToDieTransition, "die", &PursueToDieEvaluator);
    gai_fsm_add_transition(&fsm, "pursue", &PursueToDieTransition);

    gai_fsm_transition_t PursueToShootTransition;
    gai_evaluator_t PursueToShootEvaluator;
    gai_evaluator_init(&PursueToShootEvaluator, "PursueToShoot", Random_Evaluator, 0);
    gai_fsm_transition_init(&PursueToShootTransition, "shoot", &PursueToShootEvaluator);
    gai_fsm_add_transition(&fsm, "pursue", &PursueToShootTransition);

    gai_fsm_transition_t PursueToMoveTransition;
    gai_evaluator_t PursueToMoveEvaluator;
    gai_evaluator_init(&PursueToMoveEvaluator, "PursueToMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&PursueToMoveTransition, "move", &PursueToMoveEvaluator);
    gai_fsm_add_transition(&fsm, "pursue", &PursueToMoveTransition);

    gai_fsm_transition_t PursueToIdleTransition;
    gai_evaluator_t PursueToIdleEvaluator;
    gai_evaluator_init(&PursueToIdleEvaluator, "PursueToIdle", Random_Evaluator, 0);
    gai_fsm_transition_init(&PursueToIdleTransition, "idle", &PursueToIdleEvaluator);
    gai_fsm_add_transition(&fsm, "pursue", &PursueToIdleTransition);

    /* -------------------------------------------------------------------------------------------------------------- */
    /* reload */

    gai_fsm_transition_t ReloadToDieTransition;
    gai_evaluator_t ReloadToDieEvaluator;
    gai_evaluator_init(&ReloadToDieEvaluator, "ReloadToDie", Random_Evaluator, 0);
    gai_fsm_transition_init(&ReloadToDieTransition, "die", &ReloadToDieEvaluator);
    gai_fsm_add_transition(&fsm, "reload", &ReloadToDieTransition);

    gai_fsm_transition_t ReloadToShootTransition;
    gai_evaluator_t ReloadToShootEvaluator;
    gai_evaluator_init(&ReloadToShootEvaluator, "ReloadToShoot", Random_Evaluator, 0);
    gai_fsm_transition_init(&ReloadToShootTransition, "shoot", &ReloadToShootEvaluator);
    gai_fsm_add_transition(&fsm, "reload", &ReloadToShootTransition);

    gai_fsm_transition_t ReloadToPursueTransition;
    gai_evaluator_t ReloadToPursueEvaluator;
    gai_evaluator_init(&ReloadToPursueEvaluator, "ReloadToPursue", Random_Evaluator, 0);
    gai_fsm_transition_init(&ReloadToPursueTransition, "pursue", &ReloadToPursueEvaluator);
    gai_fsm_add_transition(&fsm, "reload", &ReloadToPursueTransition);

    gai_fsm_transition_t ReloadToRandomMoveTransition;
    gai_evaluator_t ReloadToRandomMoveEvaluator;
    gai_evaluator_init(&ReloadToRandomMoveEvaluator, "ReloadToRandomMove", Random_Evaluator, 0);
    gai_fsm_transition_init(&ReloadToRandomMoveTransition, "randomMove", &ReloadToRandomMoveEvaluator);
    gai_fsm_add_transition(&fsm, "reload", &ReloadToRandomMoveTransition);

    gai_fsm_transition_t ReloadToIdleTransition;
    gai_evaluator_t ReloadToIdleEvaluator;
    gai_evaluator_init(&ReloadToIdleEvaluator, "ReloadToIdle", Random_Evaluator, 0);
    gai_fsm_transition_init(&ReloadToIdleTransition, "idle", &ReloadToIdleEvaluator);
    gai_fsm_add_transition(&fsm, "reload", &ReloadToIdleTransition);
    /* -------------------------------------------------------------------------------------------------------------- */
    /*  */
    gai_fsm_set_state(&fsm, "idle", 0);

    while(!stop){
        gai_fsm_update(&fsm , 0);
    }

    gai_fsm_destroy(&fsm);

    return 0;
}