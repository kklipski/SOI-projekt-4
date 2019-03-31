# Ćwiczenie 4.

## Synchronizacja procesów z wykorzystaniem monitorów

### Cel ćwiczenia
Należy napisać program w języku C++ w środowisku systemu Linux realizujący rozwiązanie problemu zadanego przez prowadzącego ćwiczenie. Problem jest rozwinięciem ćwiczenia nr 3.

### Operacje monitorowe 
Przykładowa notacja monitora (za Hoare'em) jest następująca:
```
type mon = monitor                                 // 1
    var ... {zmienne wewnętrzne monitora};
    var c1,...,cP:condition;                       // 2
procedure entry X (...);
var ...
begin                                              // 3
    if ... then ci.wait;                           // 4
    ... {właściwa treść procedury};
    if not cj.empty                                // 5 
    then cj.signal                                 // 6
end;                                               // 7
procedure entry Y ...
...
begin 
    ... {inicjowanie zmiennych wewnętrznych};      // 8
end;
```

Do realizacji monitora służy specjalna biblioteka realizująca następujące elementy monitora, odwołujące się do wskazanych miejsc w kodzie:

**Ad 1.** Typ "mon" służący do "powoływania" monitorów, w szczególności zawierających zmienną semaforową do wzajemnego wykluczania "mutex". Każdy monitor musi zawierać dokładnie jedną zmienną typu "mon": *m:mon*;

**Ad 2.** Typ warunków Hoare'a, pozwalający na powoływanie warunków synchronizujących;

**Ad 3.** Wejście do monitora "enter". Musi być umieszczone na początku każdej metody monitora i tylko tam: *enter(m)*;

**Ad 4.** Zawieszenie na warunku: *wait(ci)*;

**Ad 5.** Badanie niepustości warunku: *empty(cj)*;

**Ad 6.** Wznawianie procesu: *signal(cj)*;

**Ad 7.** Wyjście z monitora "leave". Musi być umieszczone na końcu każdej metody monitora i tylko tam: *leave(m)*;

**Ad 8.** Inicjowanie monitora i zmiennych warunkowych: *initm(m)*, *initc(c1,m)*, ...;

### Zadanie do zrealizowania
Należy zrealizować typ "bufor komunikacyjny" przy pomocy monitora. W czasie implementacji należy zapewnić synchronizację taką samą jak w ćwiczeniu 3. Należy zrealizować więzy pomiędzy buforami, te same co w ćwiczeniu 3., zmodyfikowane o dodatkowe warunki wskazane dla ćwiczenia 4. przez prowadzącego.

#

### Dodatkowe informacje
Powyższy tekst pochodzi z instrukcji do czwartego ćwiczenia laboratoryjnego z przedmiotu Systemy operacyjne (I). Treść mojego zadania jest umieszczona [poniżej](https://github.com/kklipski/SOI-projekt-4#moje-zadanie).

### Moje zadanie
Jest danych 2 producentów, którzy produkują losowe liczby i umieszczają je w 9-cio elementowym buforze FIFO. Jest też danych 2 konsumentów, którzy konsumują produkty z bufora (usuwają je), przy czym konsument pierwszy konsumuje tylko liczby parzyste, a drugi tylko liczby nieparzyste. Ponadto konsumenci mogą konsumować tylko, jeśli w buforze znajdują się co najmniej 3 produkty. Zadanie należy wykonać korzystając z monitorów.

Koncepcja rozwiązania: [Koncepcja.pdf](Koncepcja.pdf).

*Migrated from Bitbucket.*
