damp = 0.1; 
    minf=350; 
    maxf=2250; 
    Fw = 1000;
    Fc = 2000;
    wahType=1;
    Fs=44100;
%% A. Create input and output objects
fileReader = dsp.AudioFileReader( ...
    'guitar.wav', ...
    'SamplesPerFrame',64, ...
    'PlayCount',3);

deviceWriter = audioDeviceWriter( ...
    'SampleRate',fileReader.SampleRate);

%% B. Create an object of a handle class
x = parameterRef;
x.name = 'Fc';
x.value = Fc;

%% C. Open the UI function for your parameter
parameterTuningUI(x,minf,maxf);

%% D. Process audio in a loop
while ~isDone(fileReader)
    audioIn = fileReader();
    
    drawnow limitrate
    y=EfectoWahWah(damp,minf,maxf,Fs,Fw, audioIn, x.value,wahType);
    y=y';
    %y=single(y);
    
    %y=reshape(y,[64,2]);
    disp(size(y));   % Check dimensions
    disp(size(audioIn));

    % Convert the mono output to stereo by duplicating it
    audioOut= repmat(y, 1, 2);
    
    deviceWriter(audioOut);
end

% Release input and output objects
release(fileReader)
release(deviceWriter)