   %% Time specifications:
   Fs = 100;                      % samples per second
   dt = 1/Fs;                     % seconds per sample
   StopTime = 1;                  % seconds
   t = (0:dt:StopTime-dt)';
   N = size(t,1);
   %% Sine wave:
   Fc = 12;                       % hertz
   x = 2*cos(2*pi*Fc*t);
   figure(1);
   plot(x);
   %% Fourier Transform:
   X = fftshift(fft(x));
   %% Frequency specifications:
   dF = Fs/N;                      % hertz
   f = 0:dF:Fs/2-dF;           % hertz
   %% Plot the spectrum:
   figure(2);
   plot(f,abs(X(1:N/2))/N);
   xlabel('Frequency (in hertz)');
   title('Magnitude Response');