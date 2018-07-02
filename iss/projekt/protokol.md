# ISS Projekt 
## Jakub Kulich (xkulic03@stud.fit.vutbr.cz)

1. Načítanie signálu:
Signál sme načítali pomocou príkazu 
	```
	[s, Fs] = audioread('xkulic03.wav');
	```
2. Výpočet spektra signálu: 
![](/home/jakub/fit/iss/projekt/spektrum.png) 
Pre výpočet spektra sme použili funkciu fft: 
	```
	X = fft(s);
	```
osu v Hertzoch sme si vypočítali pomocou 
	```
	f = (0:N/2-1)/N * Fs;
	```
	a	 signál vykreslili pomocou 
	```
	plot(f,abs(X(1:N/2)));
	```
3. Maximum modulu spektra vyčítame zo spektra a je vo frekvencii 790 Hz.
4. Obrázok s nulami a pólami prenosovej funkcie filtra:
![](/home/jakub/fit/iss/projekt/filterzplane.png) 
Filter je stabilný - je to možné vyčítať  z obrázku - filter je stabilný keď póly sa nachádzajú vo vnútri jednotkovej kružnice.
5. Frekvenčná charakteristika filtra: 
![](/home/jakub/fit/iss/projekt/filtercharakteristika.png) 
Z charakteristiky filtra vieme vyčítať že filter je horná priepusť, keďže spodné frekvencie sú zadržané
6. Spektrum signálu po prefiltrovaní: 
![](/home/jakub/fit/iss/projekt/spektrumfilter.png) 
Spektrum sme znova vypočítali pomocou fft, tak ako v 2. príklade
7. Maximum modulu spektra filtrovaného signálu je vo frekvencii 4273 Hz.
8. Obdĺžniky sme vyhľadali v pôvodnom signáli tak, že sme si vygenerovali signál zložený z obdĺžnikov podľa zadania a následne sme zmerali podobnosť pôvodného a vygenerovaného signálu pomocou funkcie `xcorr`. Podobnosť vykreslíme do obrázku, z ktorého následne vieme vyčítať, že obdĺžnikové impulzy sa začínajú v 10858. vzorke pôvodného signálu čo je približne v 0.68s.
	```
	obdlzniky = [1; 1; 1; 1; -1; -1; -1; -1];
	obdlzniky = repmat(obdlzniky, 40, 1);
	[c1,lag1] = xcorr(s,obdlzniky);
	plot(c1(16000:31999));
	```
![](/home/jakub/fit/iss/projekt/obdlzniky.png)
9. Obrázok autokorelačných koeficientov: *R[k], k = -50 ... 50* :
![](/home/jakub/fit/iss/projekt/korelace.png)
Autokorelačné koeficienty sme vypočítali pomocou funkcie xcorr: 
	```
	Rv = xcorr(s,'biased');
	```
koeficienty *R[k], k = -50 ... 50* sme vykreslili pomocou: 
	```
	kn = length(Rv);
	k = round(kn/2)-50:1:round(kn/2)+50;
	plot(k - round(kn/2), Rv(k));
	```
10. Hodnota korelačného koeficientu *R[10] = -0.02586*. Hodnotu sme vyčítali z obrázku alebo sme ho vypísali z vektora koeficientov pomocou 
	```
	Rv(round(kn/2) + 10)
	```
11. Časový odhad združenej funkcie hustoty rozdelenia pravdepodobnosti *p(x1,x2,10)* medzi vzorkami *n* a *n + 10*:
![](/home/jakub/fit/iss/projekt/pravdepodobnost.png) 
Časový odhad sme urobili cez funkciu `hist2opt` kde ako argumenty sme vložili pôvodný signál, signál posunutý o 10 vzorkov a tretí parameter bol `x=linspace(min(s),max(s),64);` 
12. Ak chceme overiť či funkcia hustoty rozdelenia pravdepodobnosti je správna, je potrebné vypočítať súčet všetkých hodnôt a vynásobiť ho obsahom jedného štvorca - musí byť rovné 1: 
	```
	sum(sum(p))*surf
	```
Výsledok nám vyšiel 1, takže funkcia je správna.
13. Autokorelačný koeficient *R[10]* nám vrátila funkcia `hist2opt` a jeho hodnota je *R[10] = -0.0258*. V porovnaní s výsledkom z 
príkladu 10 je rozdiel minimálny - až v desattisícinách.