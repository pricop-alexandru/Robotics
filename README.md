# 1. Quick Time

 In aceasta lucrare vom face un joc de tip reflex, in care folosim comunicarea a 2 placi de arduino cu microchipul ATMega328P, folosind SPI si o comunicare de tip master si slave. Fiecare jucator va avea butoane si LED-uri proprii, iar jocul se va desfasura in mai multe runde. Scopul fiecarui jucator este sa apese cat mai rapid butonul care corespunde culorii afisate pe LED-ul RGB al echipei sale. Punctajul fiecarui jucator va fi afisat pe un ecran LCD si se va actualiza pe parcursul jocului. La finalul jocului, jucatorul cu cel mai mare punctaj este declarat castigator.

# 2. Simularea temei
 ![Simularea Wokwi:](https://github.com/pricop-alexandru/Robotics/blob/tema-3/images/simulrarewokwi3.png?raw=true)
  
  Aceasta este simularea temei in Wokwi, care simuleaza circuitul in felul urmator:
A. Jocul pornește cu afișarea unui mesaj de bun venit pe LCD. Apăsarea unui buton declanșează startul jocului.

Pentru începerea jocului, butonul de start poate fi implementat într-un mod flexibil, rămânând la latitudinea studenților să aleagă una dintre următoarele variante:

- în această variantă, jocul pornește la apăsarea oricărui buton.
- Un buton specific începe jocul* - se poate desemna un buton anume, clar marcat pe breadboard, pentru a porni jocul.
- Un al 7-lea buton dedicat* – se poate adăuga un buton suplimentar destinat exclusiv pornirii jocului.

B. Fiecare jucător are trei butoane, fiecare asociat unui LED de o culoare diferită și un al 4-lea LED RGB.
- La fiecare rundă, fiecare jucător este cel activ.
- LED-ul RGB al jucătorului activ se aprinde într-o culoare corespunzătoare unuia dintre butoanele sale. Jucătorul trebuie să apese cât mai rapid butonul care corespunde culorii LED-ului RGB, pentru a obține puncte. - Cu cât reacționează mai repede, cu atât primește mai multe puncte.
- La finalul unei runde LCD-ul afișează punctajul actualizat al ambilor jucători.
- Pe tot parcursul jocului display-ul LCD va arata punctajul fiecarui jucator.

C. Servomotorul se rotește pe parcursul jocului, indicând progresul. O rotație completă a servomotorului marchează sfârșitul jocului (voi decideti cat de repede se misca).
- La final, LCD-ul afișează numele câștigătorului și scorul final pentru câteva secunde, apoi revine la ecranul de start cu mesajul de bun venit.

# 3. Realizarea circuitului si detaliile tehnice.
SPI: Tema aceasta implică foarte multe legături. Atât de multe încât un singur arduino uno nu ne oferă suficienți pini GPIO. De aceea pentru această temă este nevoie de 2 arduino uno care vor comunica folosind SPI. Arduinoul master va fi cel responsabil pentru controlul LCD-ului, servomotorului și va fi cel care ține în memorie starea jocului (ex. punctajul fiecărui jucator, ledul care trebuie să fie aprins acum, etc.). Arduino-ul slave va controla butoanele și ledurile, primind mesaje de la arduinoul master pentru a ști ce led să aprindă și trimițând înapoi mesaje despre butonul apăsat.
Butoane:

- Pentru începerea jocului butonul de start poate fi implementat în diverse moduri:
- Orice buton începe jocul
- Un anume buton începe jocul (ar trebui să fie clar pe breadboard care este acel buton care pornește jocul)• Un al 7-lea buton dedicat poate fi pus pentru pornirea jocului
- Cât timp jocul este în desfășurare butoanele trebuie să poată fi folosite doar pentru controlul jocului și să nu reseteze progresul
- Doar butoanele jucătorului din acea rundă trebuie să poată controla jocul
- Chiar și cu 2 plăci arduino nu avem suficienți pini pentru toate componentele. De aceea, putem multiplexa butoanele folosind rezistențe.

LED-uri:

- Fiecare buton are asociat un LED de o culoare diferită. Pe parcursul jocului acestea trebuie să fie aprinse pentru a vedea cărei culoare îi corespunde fiecare buton
- LED-ul rgb trebuie să se aprindă în una din cele 3 culori ale butoanelor
- LED-ul rgb trebuie să fie stins dacă nu este runda jucătorului corespunzător acelui LED

LCD:

- Pentru controlul acestuia ne putem folosi de biblioteca LiquidCrystal
- Trebuie să aibă setată o luminozitate și un contrast suficient de bune cât să fie vizibil textul pe ecran.
- Vor fi folosiți doar pini D4-7 pentru liniile de date ale ecranului
- Pe parcursul jocului trebuie să afișeze punctajele celor 2 jucători

Servomotorul va începe de la poziția de 0 grade și se va deplasa în sens antiorar pentru a indica scurgerea timpului.


 In aceasta lucrare avem la dispozitie doua placi arduino uno incorporate cu microchip-ul ATMega328P, astfel folosind urmatoarea configuratie in platformio.ini:
```
[env:ATmega328P]
platform = atmelavr
board = ATmega328P
framework = arduino
upload_protocol = urclock
board_build.f_cpu = 16000000L
monitor_speed = 9600
monitor_echo = true
lib_deps = 
	arduino-libraries/Servo@^1.2.2
	fmalpartida/LiquidCrystal@^1.5.0
```
A se nota faptul ca setam si baudrate-ul cu monitor_speed la viteza necesara, si viteza de cpu al placilor ATmega328P este de 16M.
<img src="https://github.com/pricop-alexandru/Robotics/blob/tema-3/images/pozaspeedgame1.jpeg" width="300">

# 4. Programarea experimentului:
Componente:
2x LED RGB (pentru cei 2 jucatori)
7x Butoane (unul dedicat pentru start/stop si celelalte pentru apasarile corecte)
21x Rezistoare (13x 220 ohmi, 8x 330 ohmi)
2x Breadboard
Linii de legătură (foarte multe)
2 placi Arduino Uno cu ATMega328P, de la 2 kit-uri

Codul folosit se gaseste in https://github.com/pricop-alexandru/Robotics/blob/tema-3/master.cpp , respectiv https://github.com/pricop-alexandru/Robotics/blob/tema-3/slave.cpp.

<img src="https://github.com/pricop-alexandru/Robotics/blob/tema-3/images/pozaspeedgame2.jpeg" width="300"> <img src="https://github.com/pricop-alexandru/Robotics/blob/tema-3/images/pozaspeedgame3.jpeg" width="300"> <img src="https://github.com/pricop-alexandru/Robotics/blob/tema-3/images/pozaspeedgame4.jpeg" width="300">

