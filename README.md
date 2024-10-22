# 1. Sistem de incarcare al unui vehicul electric

 În această temă trebuie să simulăm o stație de încărcare pentru un vehicul electric, folosind mai multe LED-uri și butoane. În cadrul acestui task trebuie să ținem cont de stările butonului și să folosim debouncing, dar și să coordonăm toate componentele ca într-un scenariu din viața reală.

# 2. Simularea temei
 ![alt text](https://github.com/pricop-alexandru/Robotics/blob/tema-1/images/image_2024-10-22_135803760.png?raw=true)
  Aceasta este simularea temei in TinkerCAD, care simuleaza circuitul in felul urmator:
 - Led-ul RGB reprezintă disponibilitatea stației. Dacă stația este liberă led-ul va fi verde, iar dacă stația este ocupată se va face roșu.
 - Led-urile simple reprezintă gradul de încărcare al bateriei, pe care îl vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se încărca prin aprinderea succesivă a led-urilor, la un interval fix de 3s. LED-ul care semnifică procentul curent de încărcare va avea starea de clipire, LED-urile din urma lui fiind aprinse continuu, iar celelalte stinse.
 - Apăsarea scurtă a butonului de start va porni încărcarea. Apăsarea acestui buton în timpul încărcării nu va face nimic.
 - Apăsarea lungă a butonului de stop va opri încărcarea forțat și va reseta stația la starea liberă. Apăsarea acestui buton cat timp stația este liberă nu va face nimic.
# 3. Realizarea circuitului:
 In aceasta lucrare avem la dispozitie o placa arduino uno incorporata cu microchip-ul ATMega328P, astfel folosind urmatoarea configuratie in platformio.ini:
```
[env:ATmega328P]
platform = atmelavr
board = ATmega328P
framework = arduino
upload_protocol = urclock
board_build.f_cpu = 12000000L
```
<img src="https://github.com/pricop-alexandru/Robotics/blob/tema-1/images/pozaincarcare1.jpeg" width="300">

# 4. Programarea experimentului:
Componente:
4x LED-uri (pentru a simula procentul de încărcare)
1x LED RGB (pentru starea de liber sau ocupat)
2x Butoane (pentru start încărcare și stop încărcare)
9x Rezistoare (7x 220/330ohm, 2x 1K)
Breadboard
Linii de legătură


<img src="https://github.com/pricop-alexandru/Robotics/blob/tema-1/images/pozaincarcare2.jpeg" width="300"> <img src="https://github.com/pricop-alexandru/Robotics/blob/tema-1/images/pozaincarcare3.jpeg" width="300"> <img src="https://github.com/pricop-alexandru/Robotics/blob/tema-1/images/pozaincarcare4.jpeg" width="300">

https://youtube.com/shorts/5vkmwre_HtM?feature=share
