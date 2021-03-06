# Program Overview #
This program evaluates candidate routines for Robotics Process Automation. 
It analyses the input stream, identifying the initial variables of interest, actions (including the preconditions and effects of each action), 
trace (composed of a sequence of actions) and candidate routines. 

Stage 0 involves the initial analysis. It displays the number of distinct actions, length of the input trace and whether or not the given input trace is valid. 
The initial state of the 26 Boolean variables (from 'a' to 'z') are displayed, followed by the changes to these variables after each action in the input trace is executed.

Stage 1 looks at the candidate routines (which are also a sequence of actions, similar to the trace) and for each candidate routine, it displays all the non-overlapping
sub-sequences of consecutive actions of the trace that produce the same cumulative effect. At this stage, the sub-sequences identified, if executed, 
must set the same Boolean variables to the same values as the candidate routine (without  manipulating any other Boolean variable).

Stage 2 is very similar to Stage 1 as it also has to identify sub-sequences of the trace that produce the same cumulative effect as the given candidate routines. However,
the sub-sequences identified this time can manipulate other Boolean variables, as long as those variables are returned to their initial state. 

## Note ##

This program was written as an assignment for the subject COMP10002 Foundations of Algorithms at the University of Melbourne. The skeleton code has been written by 
Artem Polyvyanyy (artem.polyvyanyy@unimelb.edu.au).
