
%% A. Create input and output objects
fileReader = dsp.AudioFileReader( ...
    'guitar.wav', ...
    'SamplesPerFrame',64, ...
    'PlayCount',3);
deviceWriter = audioDeviceWriter( ...
    'SampleRate',fileReader.SampleRate);

%% B. Create an object of a handle class
x = parameterRef;
x.name = 'gain';
x.value = 2.5;

%% C. Open the UI function for your parameter
parameterTuningUI(x,0,5);

%% D. Process audio in a loop
while ~isDone(fileReader)
    audioIn = fileReader();
    
    drawnow limitrate
    audioOut = audioIn.*x.value;
    
    deviceWriter(audioOut);
end

% Release input and output objects
release(fileReader)
release(deviceWriter)