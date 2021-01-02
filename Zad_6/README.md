# README 
#### Pracownia 5 Podstawy Grafiki Komputerowej - bąbelki
##### Maksymilian Czudziak 309498

## Uruchamianie
1. `make`
2. `./aquarium`

## Sterowanie
W/S/A/D - ruch przód/tył/lewo/prawo
R/F - ruch góra/dół
E - przełączenie widoku pierwszoosobowego i rzutu z góry
L - pełny ekran
Q - wyjście z programu

## Cechy zaimplementowanego oświetlenia
- Model Phonga (Ambient + Diffuse + Specular) dla bąbelków i ścian akwarium
- Absorbcja barw im dalej od punktu patrzenia - prawo Beera
- Oświetlenie z punktu w którym znajduje się gracz (o kolorze sfery reprezentującej jego pozycję w rzucie z góry)
- Oświetlenie z punktu nad akwarium, ale nie w miejscu z którego jest rzut z góry.

## Cechy zaimplementowanego środowiska gry
- Dotknięcie bąbelka skutkuje zakończeniem gry
- Dotknięcie przeciwległej ściany skutkuje przejściem do następnego poziomu gry w którym pojawia się 10% więcej bąbelków (na kolejnych poziomach jest 40%->50%->60%->70%)
- Zamiast poziomu piątego jest zakończenie gry sukcesem