% Written by Andrew Donelick
% EELE 577 - Advanced Digital Signal Processing
% Final Project

% This script allows you to plot more than one experiment on 
% the same axes. Yes, this code is a little messy, but it
% gets the job done. 

close all;

% Define the number of experiments we want to plot on the same figure,
% as well as the number of games per trial in each experiment.
numExperiments = 3;
numGames = 100000;

% Get the first data file, corresponding to the first experiment
dataFile = input('File to plot: ', 's');
filenameParts = strsplit(dataFile, '_');

learningAlgorithm = 'None';
stateType = 'None';
plotType = 'None';

% Extract the learning algorithm used in the experiment. We assume 
% this learning algorithm was used for all experiments being plotted.
if strcmp(filenameParts{1}, 'TD')
    learningAlgorithm = 'Temporal Difference Learning';
elseif strcmp(filenameParts{1}, 'Q')
    learningAlgorithm = 'Q-Learning';
elseif strcmp(filenameParts{1}, 'EG')
    learningAlgorithm = 'Epsilon-Greedy';
end

% Get the type of state the algorithm was applied to
if filenameParts{2} == 'AS'
    stateType = 'Afterstate';
elseif filenameParts{2} == 'S'
    stateType = 'State';
end

% Set the type of plot (scores or wins)
if strcmp(filenameParts{5}, 'scores.csv')
    plotType = 'Scores';
elseif strcmp(filenameParts{5}, 'wins.csv')
    plotType = 'Wins';
end

% Load the data from each of the experiments
averageScores = zeros(numExperiments, numGames);
learningRates = zeros(1, numExperiments);

for experiment = 1:numExperiments

    learningRate = str2double(filenameParts{4}) / 1000;
    learningRates(experiment) = learningRate;

    % Load the data from the data file
    rawData = csvread(dataFile);
    [n, m] = size(rawData);

    for i = 1:(n*m/numGames)
        if (m == 1)
            averageScores(experiment, :) = averageScores(experiment, :) + reshape(rawData(numGames*(i-1)+1:numGames*i), [1, numGames]);
        else
            averageScores(experiment, :) = averageScores(experiment, :) + rawData(i, :);
        end
    end

    % Average over the experiment trials, filter the remaining results
    averageScores(experiment, :) = averageScores(experiment, :) ./ (n*m/numGames);
    averageScores(experiment, :) = filter(0.001*ones(1, 1000), 1, averageScores(experiment, :));
    
    if (experiment ~= numExperiments)
        dataFile = input('File to plot: ', 's');
        filenameParts = strsplit(dataFile, '_');
    end
end

% Set the plot's title and y-axis label
if strcmp(learningAlgorithm, 'Epsilon-Greedy')
    plotTitle = ['Epsilon Greedy Agent; \epsilon = ', num2str(learningRate)];
else
    plotTitle = [stateType, ' ', learningAlgorithm];
end

if strcmp(plotType, 'Scores')
    y_label = 'Game Score';
else
    y_label = 'Win Rate';
end

% Plot the different experiments
% Right now, you have to manually specify the legend
plot(1:numGames, averageScores(1, :), 1:numGames, averageScores(2, :), 1:numGames, averageScores(3, :));
legend('\alpha = 0.01', '\alpha = 0.005', '\alpha = 0.001');
grid;
title(plotTitle);
xlabel('Training Games');
ylabel(y_label);
