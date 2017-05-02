# 2048-rl

This repository contains code used to implement reinforcement learning agents
that attempt to learn to play the popular game 2048. The code is written in
C++ in an attempt to make the various learning experiments run as quickly as
possible. I also provide a couple Matlab scripts for visualizing the results.


## Compiling and Running the Code

The first step to complete is compiling the code. To compile all of the 
programs contained in this repository, simply open a terminal, then navigate
to the directory containing the code. Then, execute the make command:

```
2048-rl$ make
```

If you wish to only compile a specific program, you can do that too. For
instance, if you wish to compile the qLearning program, run the following
command:

```
2048-rl$ make qLearning
```

Once you have compiled the various programs within the repository, you can
do any number of things.

* **Play 2048 Manually**  
    The program `play2048` allows a user to manually play 2048. This program
    was created as a test of the game environment, but it is still fun in
    and of itself. 

* **Train a Sofware Agent**  
    The whole purpose of this project was to implement a reinforcement learning
    agent for 2048. There are three different types of software agents you can 
    train:
    * Q-Learning agent
    * Temporal difference learning agent (applied to game states)
    * Temporal difference learning agent (applied to game afterstates)

    The programs to train each of these agents are `qLearning`, `stateLearning`,
    and `afterStateLearning`, respectively. To change the parameters for each
    learning algorithm, you will need to open up the corresponding `.cpp` file
    to edit them. Don't forget to recompile the program after editing it!

    **Note:** These programs only train agents and save their performance
    metrics, such as the scores and wins as a function of training games.
    These programs do not save the agents themselves.


* **Watch an Agent Play 2048**  
    You can also train an agent, and then watch it play a game. Right now, you 
    can only train a temporal difference learning agent on game afterstates
    with this functionality. To do so, you will need to compile and run the
    `afterStateAgent` program. Like with the standard training programs, the
    training parameters are contained within the corresponding `.cpp` file.


## Viewing the Results

Once you have run some training experiments, you might want to view the 
results from the experiments. To do this, I've included a couple Matlab scripts
within the results folder.

* `plotResults`  
    This script allows you to plot the results from a single experiment. 
    You can plot either the scores of the agent, or the agent's winning rate. 

* `plotMultipleExperiments`  
    This script allows you to plot the results from a single experiment. 
    You can plot either the scores of the agent, or the agent's winning rate. 
