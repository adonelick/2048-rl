% Written by Andrew Donelick
% EELE 577 - Advanced Digital Signal Processing
% Final Project

% This script plots the results from a single experiment.

close all;

% Get the data file, corresponding experiment you wish to analyze
dataFile = input('File to plot: ', 's');
filenameParts = strsplit(dataFile, '_');

learningAlgorithm = 'None';
stateType = 'None';
plotType = 'None';

% Figure out the learning algorithm used by the experiment
if strcmp(filenameParts{1}, 'TD')
    learningAlgorithm = 'Temporal Difference Learning';
elseif strcmp(filenameParts{1}, 'Q')
    learningAlgorithm = 'Q-Learning';
elseif strcmp(filenameParts{1}, 'EG')
    learningAlgorithm = 'Epsilon-Greedy';
end

% Figure out the state type used by the learning algorithm
if filenameParts{2} == 'AS'
    stateType = 'Afterstate';
elseif filenameParts{2} == 'S'
    stateType = 'State';
end

% Determine if we will be plotting scores or winning rate
if strcmp(filenameParts{5}, 'scores.csv')
    plotType = 'Scores';
elseif strcmp(filenameParts{5}, 'wins.csv')
    plotType = 'Wins';
end

% Get the number of games per trial, and the learning rate
numGames = str2double(filenameParts{3});
learningRate = str2double(filenameParts{4}) / 1000;

% Load the data from the data file
rawData = csvread(dataFile);
[n, m] = size(rawData);

if (numGames == 0) && (m == 1)
    numGames = n;
end

% Reformat the data (csvread doesn't do a good job for large files)
averageScores = zeros(1, numGames);
for i = 1:(n*m/numGames)
    if (m == 1)
        averageScores(:) = averageScores(:) + rawData(numGames*(i-1)+1:numGames*i);
    else
        averageScores = averageScores + rawData(i, :);
    end
end

% Average over the experiment trials, filter the remaining results
averageScores = averageScores ./ (n*m/numGames);
averageScores = filter(0.001*ones(1, 1000), 1, averageScores);

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

% Plot the scores or winning rate
plot(averageScores);
grid;
title(plotTitle);
xlabel('Training Games');
ylabel(y_label);