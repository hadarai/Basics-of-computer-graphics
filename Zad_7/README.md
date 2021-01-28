# README 
#### Pracownia 7 Podstawy Grafiki Komputerowej - Viewer 3D
##### Maksymilian Czudziak 309498

## Uruchamianie
1. `make`
2. `./viewer <nazwa_pliku_obiektu>.obj <nazwa_pliku_tekstury>`

## Sterowanie
- W/S -> Zbliżanie i oddalanie do oglądanego obiektu
- A/D -> Obrót wokół obiektu - W lewo/prawo
- R/F -> ruch punktu patrzenia - wyżej/niżej na OY
- T/G -> ruch pozycji na który kamera jest zwrócona - wyżej/niżej na OY
- L -> pełny ekran
- Q -> wyjście z programu


## Uwagi eksploatacyjne
- Obsługiwane pliki obiektu to tylko .obj
- Obsługiwane i przetestowane rozszerzenia plików tekstur to `.jpg`, `.jpeg`, `.png`, `.bmp`
- Linijki `97` i `98` w pliku `LoadedObject.fragmentshader` obsługują odpowiednio nakładanie tektury i nakładanie szumu. 
- Domyślnie ustawione jest nakładanie szumu na teksturę. Nie wygląda to najlepiej, ale prezentuje to spełnienie obydwu poleceń

## Opis załączonych plików testowych
Jest taki folder jak `assets`. Tam folder `mine_cube` posiada obiekt (sześcian) i teksturę owijającą odpowiednio ten sześcian. (białe krawędzie to problem tekstury, a nie jej obsługi).
Inne assety to sześcian, torus i suzanne, prosto z blendera. A także tekstura powierzchni słońca jako przykładowa grafika wrapująca obiekt.