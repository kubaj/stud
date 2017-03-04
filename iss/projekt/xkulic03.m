% 1
[s, Fs] = audioread('xkulic03.wav');
N = length(s); % Pocet vzorkov

% 2, 3
X = fft(s); % Fourierova transformacia
f = (0:N/2-1)/N * Fs; % X osa v Hz
plot(f,abs(X(1:N/2))); xlabel('f [Hz]');
pause;

% 4
b = [0.2324 -0.4112 0.2324];
a = [1 0.2289 0.4662];
zplane(b,a);
pause;

% 5
H = freqz(b,a,512);
f2 = (0:511) / 512 * Fs / 2;
plot(f2,abs(H)); grid; xlabel('f[Hz]'); ylabel('|H(f)|');
pause;

% 6, 7
sf = filter(b,a,s);
Xf = fft(sf);
plot(f,abs(Xf(1:N/2))); xlabel('f [Hz]');
pause;

% 8
obdlzniky = [1; 1; 1; 1; -1; -1; -1; -1];
obdlzniky = repmat(obdlzniky, 40, 1);

[c1,lag1] = xcorr(s,obdlzniky);
plot(c1(16000:31999));
pause;

% 9,10
Rv = xcorr(s,'biased');
kn = length(Rv);
k = round(kn/2)-50:1:round(kn/2)+50;
plot(k - round(kn/2), Rv(k));
pause;

Rv(round(kn/2) + 10)
pause;

% 11,12
ss = s(11:16000);
ss(15991:16000) = zeros(1,10);

x=linspace(min(s),max(s),64);
[h,p,r] = hist2opt(s,ss,x);
imagesc (x,x,p); axis xy; colorbar; xlabel('n+10'); ylabel('n');
pause;
r

% 13
size = (x(2)-x(1))^2;
sum(sum(p))*size
