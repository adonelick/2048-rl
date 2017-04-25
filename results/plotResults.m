% Written by Andrew Donelick
% EELE 577 - Advanced Digital Signal Processing
% Final Project

close all;

dataFile = input('File to plot: ', 's');
filenameParts = strsplit(dataFile, '_');

learningAlgorithm = 'None';
stateType = 'None';
plotType = 'None';

if filenameParts{1} == 'TD'
    learningAlgorithm = 'Temporal Difference Learning';
elseif filenameParts{1} == 'Q'
    learningAlgorithm = 'Q-Learning';
end

if filenameParts{2} == 'AS'
    stateType = 'Afterstate';
elseif filenameParts{2} == 'S'
    stateType = 'State';
end

if strcmp(filenameParts{5}, 'scores.csv')
    plotType = 'Scores';
elseif strcmp(filenameParts{5}, 'wins.csv')
    plotType = 'Wins';
end

numGames = str2double(filenameParts{3});
learningRate = str2double(filenameParts{4}) / 1000;

% Load the data from the data file
rawData = csvread(dataFile);
[n, m] = size(rawData);

averageScores = zeros(1, numGames);
for i = 1:(n*m/numGames)
    if (n == 1)
        averageScores = averageScores + rawData(numGames*(i-1)+1:numGames*i);
    else
        averageScores = averageScores + rawData(i, :);
    end
end

averageScores = averageScores ./ (n*m/numGames);
averageScores = filter(0.01*ones(1, 100), 1, averageScores);

% rawData(2, :) = filter(0.01*ones(1, 100), 1, rawData(2, :));
% rawData(3, :) = filter(0.01*ones(1, 100), 1, rawData(3, :));
% rawData(4, :) = filter(0.01*ones(1, 100), 1, rawData(4, :));

plotTitle = [stateType, ' ', learningAlgorithm];
plotTitle = [plotTitle, '; Learning Rate ', num2str(learningRate)];

%plot(1:10000, rawData(2, :), 1:10000, rawData(3, :), 1:10000, rawData(4, :));
plot(averageScores);
title(plotTitle);