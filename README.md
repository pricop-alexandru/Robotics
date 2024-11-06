# 1. Typeracer

 In aceasta lucrare vom face un joc de tip typeracer, in ideea de a folosi notiuni precum intreruperi prin apelarea registrilor, si timere folosind prescaler.

# 2. Simularea temei
 ![alt text](https://github.com/pricop-alexandru/Robotics/blob/tema-2/images/simularewokwi2.png?raw=true)
  
  Aceasta este simularea temei in TinkerCAD, care simuleaza circuitul in felul urmator:
 - Led-ul RGB reprezinta indicatorul de stare: În starea de repaus, LED-ul va avea culoarea albă, la apăsarea butonului de start, LED-ul va clipi timp de 3 secunde, indicând o numărătoare inversă până la începerea rundei, iar in timpul unei runde: LED-ul va fi verde dacă textul introdus este corect și va deveni roșu în caz de greșeală.
 - Butonul de start/stop: Modul de repaus: dacă jocul este oprit, apăsarea butonului inițiază o nouă rundă după o numărătoare inversă de 3 secunde, in timpul unei runde: Dacă runda este activă, apăsarea butonului o va opri imediat.
 - Butonul de dificultate controlează viteza cu care apar cuvintele și poate fi folosit doar în starea de repaus, la fiecare apăsare, dificultatea se schimbă ciclind între: (Easy, Medium, Hard). La schimbarea dificultății, se trimite un mesaj prin serial: “Easy/Medium/Hard mode on!”.
# 3. Realizarea circuitului:
 In aceasta lucrare avem la dispozitie o placa arduino uno incorporata cu microchip-ul ATMega328P, astfel folosind urmatoarea configuratie in platformio.ini:
```
[env:ATmega328P]
platform = atmelavr
board = ATmega328P
framework = arduino
upload_protocol = urclock
board_build.f_cpu = 16000000L
monitor_speed = 28800
monitor_echo = true
```
A se nota faptul ca setam si baudrate-ul cu monitor_speed la viteza necesara, si viteza de cpu al placii ATmega328P este de 16M.
<img src="https://github.com/pricop-alexandru/Robotics/blob/tema-2/images/pozatyperacer1.jpeg" width="300">

# 4. Programarea experimentului:
Componente:
1x LED RGB (pentru a observa starile de raspuns in urma oricarei modificari)
2x Butoane (pentru start/stop si selectare dificultate)
5x Rezistoare (3x 220/330ohm, 2x 1K)
Breadboard
Linii de legătură
Placa Arduino Uno inclusa cu ATMega328P (bineinteles)

Codul folosit se gaseste in https://github.com/pricop-alexandru/Robotics/blob/tema-2/tema2.c .

Pasii codului:

- Jocul este în repaus. LED-ul RGB are culoarea albă.

- Se alege dificultatea jocului folosind butonul de dificultate, iar în terminal va apărea “Easy/Medium/Hard mode on!”.

- Se apasă butonul de start/stop.

- LED-ul clipește timp de 3 secunde, iar în terminal se va afișa numărătoarea înversă: 3, 2, 1.

- LED-ul devine verde și încep să apară cuvinte de tastat.

- La tastarea corectă, următorul cuvânt apare imediat. Dacă nu se tasteaza cuvântul în timpul stabilit de dificultate, va apărea un nou cuvânt.

- O greșeală face LED-ul să devină roșu. Pentru a corecta cuvântul, se va folosi tasta BackSpace.

- Dupa 30 de secunde, runda se termină, iar în terminal se va afișa scorul: numărul total de cuvinte scrise corect.

- Jocul se poate opri oricând cu butonul de start/stop.

<img src="https://github.com/pricop-alexandru/Robotics/blob/tema-2/images/pozatyperacer2.jpeg" width="300"> <img src="https://github.com/pricop-alexandru/Robotics/blob/tema-2/images/pozatyperacer3.jpeg" width="300"> <img src="https://github.com/pricop-alexandru/Robotics/blob/tema-2/images/pozatyperacer4.jpeg" width="300">

# 5. Finalizarea experimentului:

[Videoclip Venerabil cu Programul](https://studio.youtube.com/video/zTPQyuWwz-4/edit)
