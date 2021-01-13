# README 
#### Pracownia 6 Podstawy Grafiki Komputerowej - Teren
##### Maksymilian Czudziak 309498

## Uruchamianie
1. `make`
2. `./terrain ./data/ -lat 50 51 -lon 16 17`

## Sterowanie
W/S/A/D - ruch przód/tył/lewo/prawo
R/F - ruch bliżej/dalej od powierzchni mapy
T/Y - zwiększanie/zmniejszanie szczegółowości mapy (-/+ LoD)

G - przełączanie trybu automatycznego LevelOfDetail

L - pełny ekran
Q - wyjście z programu

## Dodatkowe informacje o ustawionym LoDzie
- Gdy uruchomiony jest automatyczny LoD to symbol "+"/"-" przy informacji o obecnym poziomie informuje o tym, w którą stronę automat zdecydował o zmianie poziomu.
- Obecnie LoD ustawiony jest tak, aby ilość klatek na sekundę oscylowała między 10, a 15. Dla więcej niż 15 dołoży szczegółów, a dla mniej niż 10 zmniejszy ich ilość.