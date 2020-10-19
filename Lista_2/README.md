Aby doinstalować bibliotekę epoxy, pod debian/ubuntu:
      sudo apt install libepoxy-dev

TO-DO:
- [DONE] Dodać podobną do MyCross klasę rysującą okrąg przybliżony przez n-kąt. Współrzędne punktów na okręgu (cos(t), sin(t)) należy wygenerować w funkcji setBuffers()
- [DONE] Rysować okrąg i umożliwić poruszanie okręgu klawiszami 'WSAD' (analogicznie jak można ruszać krzyżem), 
- [DONE] tak aby po najechaniu na krzyż program zakończył działanie. Czyli mamy pierwszą prostą grę.   `Tutaj nie byłem pewien, czy chodzi o to by się dotknęły (jakoś tak zrobiłem), czy jak środek krzyżyka znajdzie się wewnątrz koła.`
- [DONE] Zmodyfikować shader dla MyTri, zmieniając kolor na biały blisko środka okna czyli  współrzednych (0,0).  Należy przekazać pozycje 2d do fragment shadera (out/in) i dodać w fragment shaderze instrukcje warunkową if, język GLSL jest podobny do C.


Komentarz:

Na moim komputerze nie działa input z klawiatury (Ubuntu 20.04 LTS) strzałek i Home w GLFW, dlatego dokonane zmiany to:
- Strzałki -> IKJL
- Home -> H
- ESC -> q