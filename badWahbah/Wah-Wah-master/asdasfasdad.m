clc; clear; close all;

% Define audio parameters
fs = 44100;            % Sampling rate
frameSize = 1024;      % Frame size for smooth playback
centerFreq = 1000;     % Initial center frequency (Fc)
bandwidth = 500;       % Bandwidth
stepSize = 300;        % Frequency adjustment step
maxFc = 2250;          % Maximum center frequency
minFc = 250;           % Minimum center frequency
damp = 0.1;            % Damping factor for filter

% Load the audio file (replace with your file)
[inputAudio, fs] = audioread('guitar.wav');  % Load your audio file

% Initialize the audioDeviceWriter for playback
% Initialize the audioDeviceWriter for playback
speaker = audioDeviceWriter('SampleRate', fs);


% Initialize variables for processing and playback
processedAudio = [];
frameStart = 1;
totalFrames = length(inputAudio);

% Create figure for key press control
fig = figure('Name', 'Custom Bandpass Filter', 'NumberTitle', 'off', ...
             'KeyPressFcn', @keyPressed, 'CloseRequestFcn', @closeWindow);

global isRunning;
isRunning = true;

disp('🎵 Press ↑ to increase frequency, ↓ to decrease. Close window to stop.');
fprintf('🎛 Initial Center Frequency: %.2f Hz\n', centerFreq);

% Processing loop for each frame
while isRunning && isvalid(fig) && frameStart <= totalFrames
    % Get the current frame of audio
    frameEnd = min(frameStart + frameSize - 1, totalFrames);
    x = inputAudio(frameStart:frameEnd);

    % Compute filter coefficients dynamically
    Q1 = 2 * damp;
    F1 = 2 * sin((pi * centerFreq) / fs);

    % Apply the bandpass filter using your custom implementation
    y = zeros(size(x)); % Initialize output buffer

    % First element in the vector
    y(1) = F1 * x(1);
    yl = F1 * y(1);
    yaux = y(1);

    for n = 2:length(x)
        yh = x(n) - yl - Q1 * yaux; % High-pass
        y(n) = F1 * yh + yaux;      % Band-pass
        yl = F1 * y(n) + yl;        % Low-pass
        yaux = y(n);                % Save previous bandpass output
    end

    % Store processed audio for saving later
    processedAudio = [processedAudio; y];

    % Play the processed audio
    speaker(y);

    % Update the starting point for the next frame
    frameStart = frameEnd + 1;

    % Small pause for smooth key detection
    pause(0.005);
end

% Save processed audio to file
outputFile = 'processed_audio.wav';
audiowrite(outputFile, processedAudio, fs);
disp(['✅ Processed audio saved as: ', outputFile]);

% Clean up
release(speaker);
disp('🎵 Stopped.');

% Function to handle key presses
function keyPressed(~, event)
    global centerFreq stepSize maxFc minFc;
    % Print key pressed
    fprintf('Key Pressed: %s\n', event.Key);
    switch event.Key
        case 'uparrow'   % Increase center frequency
            centerFreq = min(centerFreq + stepSize, maxFc);
        case 'downarrow' % Decrease center frequency
            centerFreq = max(centerFreq - stepSize, minFc);
    end
    % Print the updated center frequency
    fprintf('🎛 Center Frequency: %.2f Hz\n', centerFreq);
end

% Function to stop the loop when window is closed
function closeWindow(src, ~)
    global isRunning;
    isRunning = false;
    delete(src); % Close the figure
end
