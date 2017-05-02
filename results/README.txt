Results

This folder will contain the results of the various learning algorithms.
Each file saved in this folder will be named according to its contents,
as well as the agent that created its contents.

Each file has the following name structure:

[Learning Algorithm]_[State Type]_[Trial Length]_[Learning Rate]_[Contents].csv


The first field (Learning Algorithm) tells you what type of learning algorithm
was used to generate the results. Right now, there are three possible values:

    TD: Temporal difference learning
    Q: Q-learning
    EG: Epsilon greedy (not a learning algorithm, technically)


The second field (State Type) tells you whether the

    AS: Afterstates
    S: States


The third field (Trial Length) tells you how many games defined a training trial.


The fourth field tells you the learning rate. Divide this number by 1000 to get
the actual learning rate used.


Finally, the fifth field tells you whether the file contains games scores or game
wins acheived by the agent. 
