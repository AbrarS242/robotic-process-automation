/* Program to evaluate candidate routines for Robotic Process Automation.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2020, with the intention that it be modified by students
  to add functionality, as required by the assignment specification. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

/* #define provided as part of the initial skeleton --------------------------*/

#define ASIZE 26

/* my #defines's -------------------------------------------------------------*/

#define LOWER_ASCII 97  /* ASCII value of character 'a' */
#define UPPER_ASCII 65  /* ASCII value of character 'a' */
#define FALSE 0         /* false state */
#define TRUE 1          /* true state */
#define MOVE 2          /* number of characters that have to be read 
                           to move to the next stage */

/* type definitions ----------------------------------------------------------*/

// state - values of the 26 Boolean variables (Author: Artem Polyvyanyy)
typedef unsigned char state_t[ASIZE];

// action
// Original Author: Artem Polyvyanyy
// Modified By: Abrar Yusuf
typedef struct action action_t;
struct action {
    char name;        // action name
    state_t t_precon; // precondition (values that have to already be TRUE)
    int len_t_precon; // buddy variable for the TRUE preconditions
    state_t f_precon; // precondition (values that have to already be FALSE)
    int len_f_precon; // buddy variable for the FALSE preconditions
    state_t t_effect; // effect (values that have to be set to TRUE)
    int len_t_effect; // buddy variable for the TRUE effects
    state_t f_effect; // effect (values that have to be set to FALSE)
    int len_f_effect; // buddy variable for the FALSE effects
};

// step in a trace (Author: Artem Polyvyanyy)
typedef struct step step_t;
struct step {
    action_t *action; // pointer to an action performed at this step
    step_t   *next;   // pointer to the next step in this trace
};

// trace - implemented as a linked list (Author: Artem Polyvyanyy)
typedef struct {
    step_t *head;     // pointer to the step in the head of the trace
    step_t *tail;     // pointer to the step in the tail of the trace
} trace_t;

/* function prototypes  (Author: Artem Polyvyanyy) ---------------------------*/

trace_t* make_empty_trace(void);
trace_t* insert_at_tail(trace_t*, action_t*);
void free_trace(trace_t*);

/* my function prototypes ----------------------------------------------------*/

void print_stage_zero(int num_distinct, int trace_len, int val_len, 
	state_t initial_state, state_t main_state, trace_t*);
void stage_one(char c, state_t initial_state, trace_t*,  int val_len,
	action_t action_list[]); 
void stage_two(char c, state_t initial_state, trace_t*,  int val_len,
	action_t action_list[]);
void initialise_vars(state_t main_state);
void print_array(state_t array);
void traverse_trace(trace_t*, int val_len);
void array_copy(state_t array_1, state_t array_2);
void cumulative_effect(trace_t*, int check_len, int start_point, 
	state_t t_vars, state_t f_vars);
void cumulative_effect_mod(trace_t*, state_t t_vars_cand, state_t f_vars_cand, 
	int check_len, int start_point, state_t t_vars_sub, state_t f_vars_sub,
	state_t initial_state);
void print_sequence(trace_t*, int check_len, int start_point);
void array_reset(state_t array);
int cmp_arrays(state_t array_1, state_t array_2);
int get_action(action_t action_list[]);
int valid_set(trace_t*, state_t main_state, int output);
int mygetchar(void);

/* where it all happens ------------------------------------------------------*/

int
main(int argc, char *argv[]) {
	int i, num_distinct=0, stop=FALSE, 
	action_index, trace_len=0, val_len, output;
	char c;
    state_t main_state = {0}, initial_state = {0};
    action_t action_list[ASIZE];
    trace_t *trace;
    
    /* Initialise the values of the 26 boolean variables */
    initialise_vars(main_state);
    
    /* Store the actions in an array and find the number of distinct actions */
    while (stop==FALSE){
    	num_distinct++;
    	stop = get_action(action_list);
    }
    num_distinct--; 
       
    if ((c=mygetchar()) == '\0'){
    	exit(EXIT_FAILURE);	
    }
    
    /* Store the trace in a linked list */
    trace = make_empty_trace();
    while ((c=mygetchar()) != '\n'){
    	action_index = c - UPPER_ASCII;
    	trace = insert_at_tail(trace, &action_list[action_index]);
    	trace_len++;
    }
    
    /* Store a copy of the initial values of the 26 boolean variables */
    array_copy(main_state, initial_state);
   
    /* Check through the trace to see if it is valid and only execute the
       valid actions */
    output = FALSE;
    val_len = valid_set(trace, main_state, output);
    
    /* Print the outputs for Stage 0 */
    print_stage_zero(num_distinct, trace_len, 
    	val_len, initial_state, main_state, trace);
    
    /* Check if there are any further inputs */
    if ((c=mygetchar()) != '#'){
    	free_trace(trace);
    	return EXIT_SUCCESS;	
    }
    
    /* Execute Stage 1 */ 
    for (i=0; i<MOVE; i++){
		if ((c=mygetchar()) == '\0'){
			exit(EXIT_FAILURE);	
		}
    }
    printf("==STAGE 1===============================\n");
    while (c >= 'A' && c <= 'Z'){
    	stage_one(c, initial_state, trace, val_len, action_list);
    	c = mygetchar();
    	if (c >= 'A' && c <= 'Z'){
			printf("\n----------------------------------------");
    	}
    	printf("\n");
    }
    
    /* Check if there are any further inputs */
    if (c != '#'){
    	free_trace(trace);
    	return EXIT_SUCCESS;	
    }
    
    /* Execute Stage 2 */
    for (i=0; i<MOVE; i++){
		if ((c=mygetchar()) == '\0'){
			exit(EXIT_FAILURE);	
		}
    }
    printf("==STAGE 2===============================\n");
    while (c >= 'A' && c <= 'Z'){
    	stage_two(c, initial_state, trace, val_len, action_list);
    	c = mygetchar();
    	if (c >= 'A' && c <= 'Z'){
			printf("\n----------------------------------------");
    	}
    	printf("\n");
    }
    printf("==THE END===============================");
    printf("\n");
    
    free_trace(trace);
    return EXIT_SUCCESS;        // we are done !!! algorithms are fun!!!
}

/* function definitions (Author: Artem Polyvyanyy) ---------------------------*/

// Adapted version of the make_empty_list function by Alistair Moffat:
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
// Data type and variable names changed
trace_t
*make_empty_trace(void) {
    trace_t *R;
    R = (trace_t*)malloc(sizeof(*R));
    assert(R!=NULL);
    R->head = R->tail = NULL;
    return R;
}

// Adapted version of the insert_at_foot function by Alistair Moffat:
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
// Data type and variable names changed
trace_t 
*insert_at_tail(trace_t* R, action_t* addr) {
	
    step_t *new;
    new = (step_t*)malloc(sizeof(*new));
    assert(R!=NULL && new!=NULL);
    
    new->action = addr;
    new->next = NULL;
    if (R->tail==NULL) { /* this is the first insertion into the trace */
        R->head = R->tail = new; 
    } else {
        R->tail->next = new;
        R->tail = new;
    }
    return R;
}

// Adapted version of the free_list function by Alistair Moffat:
// https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
// Data type and variable names changed
void
free_trace(trace_t* R) {
    step_t *curr, *prev;
    assert(R!=NULL);
    curr = R->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(R);
}

/* my function definitions ---------------------------------------------------*/

// Reads a character from stdin
// Throws away any CR characters if they are encountered
int
mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}

// Prints the required outputs for Stage 0
void
print_stage_zero(int num_distinct, int trace_len, int val_len, 
	state_t initial_state, state_t main_state, trace_t* trace){
    int output=FALSE;
    /* Print some basic information about the trace */
    printf("==STAGE 0===============================\n");
    printf("Number of distinct actions: %d\n", num_distinct);
    printf("Length of the input trace: %d\n", trace_len);
    if (val_len == trace_len){
    	printf("Trace status: valid\n");
    } else {
    	printf("Trace status: invalid\n");
    }
    printf("----------------------------------------\n");
    
    /* Print the initial state of the 26 boolean variables and 
       their states after the execution of each valid action */
     printf("  abcdefghijklmnopqrstuvwxyz\n");
     printf("> ");
     print_array(initial_state);
     printf("\n");
     output = TRUE;
     array_copy(initial_state, main_state);
     val_len = valid_set(trace, main_state, output);
     if (val_len == '\0'){
    	exit(EXIT_FAILURE) ;	
     }
     return;
}

// Stage 1:
// Checks each candidate routine
// Finds sub-sequences in the trace which produce the same cumulative effect
// (without modifiying any other variables)
void
stage_one(char c, state_t initial_state, trace_t *trace,  int val_len,
	action_t action_list[]){ 
	int action_index, cand_len=0, curr_point, start_point=0, sub_len,
	t_match, f_match;
	state_t t_vars_cand = {0}, f_vars_cand = {0},
	t_vars_sub = {0}, f_vars_sub = {0};
	trace_t *cand_rt;
	
	/* Store current candidate routine in a linked list */
    cand_rt = make_empty_trace();
    while (c != '\n'){
    	action_index = c - UPPER_ASCII;
    	cand_rt = insert_at_tail(cand_rt, &action_list[action_index]);
    	cand_len++;
    	c=mygetchar();
    }
    
    /* Find the cumulative effects of the candidate routine */
    cumulative_effect(cand_rt, cand_len, start_point, t_vars_cand, f_vars_cand);
    
    /* Print the candidate routine */
    printf("Candidate routine: ");
    print_sequence(cand_rt, cand_len, start_point); 
    
    /* Derive subsequences of the trace */
    curr_point = 0;
    while (curr_point < val_len){
    	sub_len = 1;
    	while (sub_len <= (val_len - curr_point)){
    		/* Check whether the cumulative effect of the sub-sequence
    		   matches that of the candidate routine (without modifying
    		   any other values) */
    		cumulative_effect(trace, sub_len, 
    			curr_point, t_vars_sub, f_vars_sub); 
    		t_match = cmp_arrays(t_vars_sub, t_vars_cand);
    		f_match = cmp_arrays(f_vars_sub, f_vars_cand);
    		if (t_match && f_match){
    			printf("\n%5d: ", curr_point);
    			print_sequence(trace, sub_len, curr_point); 
    			curr_point += sub_len - 1;
    			/* Reset array values so that the next sub-sequence 
    			   can be checked */
    			array_reset(t_vars_sub);
    			array_reset(f_vars_sub);
    			break;
    		} 
    		/* Reset array values so that the next sub-sequence 
    		   can be checked */
    		array_reset(t_vars_sub);
    		array_reset(f_vars_sub);
    		sub_len++;
    	}
    	curr_point++;
    }
	free_trace(cand_rt);
} 

// Stage 2:
// Checks each candidate routine
// Finds sub-sequences in the trace which produce the same cumulative effect
// (while modifying other values that in the end return to their initial state)
void
stage_two(char c, state_t initial_state, trace_t *trace,  int val_len,
	action_t action_list[]){ 
	int action_index, cand_len=0, curr_point, start_point=0, sub_len,
	t_match, f_match;
	state_t t_vars_cand = {0}, f_vars_cand = {0},
	t_vars_sub = {0}, f_vars_sub = {0};
	trace_t *cand_rt;
	
	/* Store current candidate routine in a  linked list */
    cand_rt = make_empty_trace();
    while (c != '\n'){
    	action_index = c - UPPER_ASCII;
    	cand_rt = insert_at_tail(cand_rt, &action_list[action_index]);
    	cand_len++;
    	c=mygetchar();
    }
    
    /* Find the cumulative effects of the candidate routine */
    cumulative_effect(cand_rt, cand_len, start_point, t_vars_cand, f_vars_cand);
    
    /* Print the candidate routine */
    printf("Candidate routine: ");
    print_sequence(cand_rt, cand_len, start_point); 
    
    /* Derive subsequences of the trace */
    curr_point = 0;
    while (curr_point < val_len){
    	sub_len = 1;
    	while (sub_len <= (val_len - curr_point)){
    		/* Check whether the cumulative effect of the sub-sequence
    		   matches that of the candidate routine (accounting for
    		   other modified values that return to their initial state) */  
    		cumulative_effect_mod(trace, t_vars_cand, f_vars_cand, sub_len, 
    			curr_point, t_vars_sub, f_vars_sub, initial_state); 
    		t_match = cmp_arrays(t_vars_sub, t_vars_cand);
    		f_match = cmp_arrays(f_vars_sub, f_vars_cand);
    		if (t_match && f_match){
    			printf("\n%5d: ", curr_point);
    			print_sequence(trace, sub_len, curr_point); 
    			curr_point += sub_len - 1;
    			/* Reset array values so that the next sub-sequence 
    			   can be checked */
    			array_reset(t_vars_sub);
    			array_reset(f_vars_sub);
    			break;
    		} 
    		/* Reset array values so that the next sub-sequence 
    		   can be checked */
    		array_reset(t_vars_sub);
    		array_reset(f_vars_sub);
    		sub_len++;
    	}
    	curr_point++;
    }
	free_trace(cand_rt);
} 

// Reads the first part of the input for Stage 0
// Initialise the specified boolean variables to True
void
initialise_vars(state_t main_state){
	char c;
	int index;
	while ((c=mygetchar())!= '#'){
		if (c != '\n') {
			index = c - LOWER_ASCII;
			main_state[index] = TRUE;		
		}
	}
	if ((c=mygetchar()) == '\0'){
    	exit(EXIT_FAILURE);	
    }
	return;
}

// Prints an array with a size of 26 (corresponding to the 26 boolean variables)
void
print_array(state_t array){
	int i;
    for (i=0; i<ASIZE; i++){
    	printf("%d", array[i]);
    }
    return;
}

// Gets the information for an action and stores them in a struct
int
get_action(action_t action_list[]){
	char c, t_precon_temp[ASIZE], f_precon_temp[ASIZE];
	int i, index, stop=FALSE, nt_precon=0, nf_precon=0, nt_effect=0,
	nf_effect = 0;
	
	/* Check to see if the all the actions have been read at this point */
	c = mygetchar();
	if (c == '#'){
		stop = TRUE;
		return stop;
	} 
	
	/* Temporarily store the precondition variables until the action name
	   is found */
	i = 0; 
	if (c == ':'){
		while ((c=mygetchar())!=':'){
		f_precon_temp[i] = c;
		i++;
		nf_precon++;
		}
	} else {
		t_precon_temp[i] = c;
		i++;
		nt_precon++;
		while ((c=mygetchar())!=':'){
			t_precon_temp[i] = c;
			i++;
			nt_precon++;
		} 
		i = 0;
		while ((c=mygetchar())!=':'){
			f_precon_temp[i] = c;
			i++;
			nf_precon++;
		}
	}
	
	/* Store the action name in the struct */
	c=mygetchar();
	index = c - UPPER_ASCII;
	action_list[index].name = c;
	 if ((c=mygetchar()) == '\0'){
    	exit(EXIT_FAILURE);	
    }
	
	/* Store the precondition variables in the struct */
	for (i=0; i<nt_precon; i++){
		action_list[index].t_precon[i] = t_precon_temp[i];
	}
	action_list[index].len_t_precon = nt_precon;
	for (i=0; i<nf_precon; i++){
		action_list[index].f_precon[i] = f_precon_temp[i];
	}
	action_list[index].len_f_precon = nf_precon;
	
	/* Store the effect variables in the struct */
	i = 0;
	while ((c=mygetchar())!=':'){
		action_list[index].t_effect[i] = c;
		i++;
		nt_effect++;
	} 
	action_list[index].len_t_effect = nt_effect;
	i = 0;
	while ((c=mygetchar())!='\n'){
		action_list[index].f_effect[i] = c;
		i++;
		nf_effect++;
	}
	action_list[index].len_f_effect = nf_effect;
	
	return stop;
}

// Copies the contents of Array 1 into Array 2 
void
array_copy(state_t  array_1, state_t array_2){
	int i;
	for (i=0; i<ASIZE; i++){
		array_2[i] = array_1[i];
	}
}

// Checks how much of the input trace is valid
// Executes all the valid actions
int 
valid_set(trace_t *R, state_t main_state, int output){
	int val_len=0, i, index, len_t_precon, len_f_precon,
	len_t_effect, len_f_effect;
	char letter;
	step_t *p;
	p = R->head;
	/* Check through each action in the trace */
	while (p){
		/* Check if all the precondition variables are set to True */
		len_t_precon = p->action->len_t_precon;
		if (len_t_precon > 0){
			for (i=0; i<len_t_precon; i++){
				letter = p->action->t_precon[i];
				index = letter - LOWER_ASCII;
				if (main_state[index] != TRUE){
					return val_len;
				}	
			}
		} 
		/* Check if all the precondition variables are set to False */
		len_f_precon = p->action->len_f_precon;
		if (len_f_precon > 0){
			for (i=0; i<len_f_precon; i++){
				letter = p->action->f_precon[i]; 
				index = letter - LOWER_ASCII; 
				if (main_state[index] != FALSE){
					 return val_len; 
				}	
			}
		} 
		val_len++;
		/* Apply the effect of setting the specified variables to True */
		len_t_effect = p->action->len_t_effect;
		if (len_t_effect > 0){
			for (i=0; i<len_t_effect; i++){
				letter = p->action->t_effect[i];
				index = letter - LOWER_ASCII;
				main_state[index] = TRUE;
			}
		} 
		/* Apply the effect of setting the specified variables to False */
		len_f_effect = p->action->len_f_effect;
		if (len_f_effect > 0){
			for (i=0; i<len_f_effect; i++){
				letter = p->action->f_effect[i];
				index = letter - LOWER_ASCII;
				main_state[index] = FALSE;
			}
		}
		/* Print out the state of the 26 boolean variables after the execution
		   of the action (if required) */
		if (output == TRUE){
			printf("%c ", p->action->name);
			print_array(main_state);
			printf("\n");
		}
		p = p->next;
	}
	return val_len;
} 

// Finds the cumulative effect of the candidate routine or trace sub-sequence
void
cumulative_effect(trace_t *R, int check_len, int start_point,
	state_t t_vars, state_t f_vars){
	int i, index;
	char curr_var;
	step_t *p;
	p = R->head;

	/* Start at the specified index of the sequence */
	while (start_point != 0){
		p = p->next;
		start_point--;
	}
	
	/* Traverse through each node */
	while (check_len != 0){
		/* Track which varibales are set to True */
		for (i=0; i<(p->action->len_t_effect); i++){
			curr_var = 	p->action->t_effect[i];
			index = curr_var - LOWER_ASCII;
			/* Check if it had been set to False before */
			if (f_vars[index] == 1){
				f_vars[index] = 0;
			}
			t_vars[index] = 1;
		}	
		/* Track which varibales are set to False */
		for (i=0; i<(p->action->len_f_effect); i++){
			curr_var = 	p->action->f_effect[i];
			index = curr_var - LOWER_ASCII;
			/* Check if it had been set to True before */
			if (t_vars[index] == 1){
				t_vars[index] = 0;
			}
			f_vars[index] = 1;
		}
		
		p = p->next;
		check_len--;
	}
	return;
}

// Finds the cumulative effect of the trace sub-sequence
// Accounts for other modified variables that return to their initial state
void 
cumulative_effect_mod(trace_t *R, state_t t_vars_cand, state_t f_vars_cand, 
	int check_len, int start_point, state_t t_vars_sub, state_t f_vars_sub,
	state_t initial_state){
	int i;

	/* Find the overall cumulative effect of the trace */
	cumulative_effect(R, check_len, start_point, t_vars_sub, f_vars_sub);
	
	/* Check if any of the variables return to their initial state */
	for (i=0; i<ASIZE; i++){
		if (t_vars_sub[i] == 1 && (t_vars_cand[i] == 0 && f_vars_cand[i] == 0)){
			if (initial_state[i] == TRUE){
				t_vars_sub[i] = 0;	
			}
		}
		if (f_vars_sub[i] == 1 && (t_vars_cand[i] == 0 && f_vars_cand[i] == 0)){
			if (initial_state[i] == FALSE){
				f_vars_sub[i] = 0;	
			}
		}
	}
	return;
}

// Prints the candidate routine or trace sub-sequence provided
void
print_sequence(trace_t *R, int check_len, int start_point){
	step_t *p;
	char curr_action;
	p = R->head;
	/* Start at the specified index of the sequence */
	while (start_point != 0){
		p = p->next;
		start_point--;
	}
	
	/* Print each action in the sequence */
	while (check_len != 0){
		curr_action = p->action->name;
		printf("%c", curr_action);
		p = p->next;
		check_len--;
	}
	return;
}

// Checks if two arrays are identical
int 
cmp_arrays(state_t array_1, state_t array_2){
	int i, match=TRUE;
	for (i=0; i<ASIZE; i++){
		if (array_1[i] != array_2[i]){
			match = FALSE;
			return match;
		}
	}
	return match;
}

// Sets all values of the array to 0
void
array_reset(state_t array){
	int i;
	for (i=0; i<ASIZE; i++){
		array[i] = 0;	
	}
}