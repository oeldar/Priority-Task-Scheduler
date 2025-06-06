# Task Scheduler

Implementacija task scheduler-a sa thread pool-om u C++. Program omogućava raspoređivanje i izvršavanje zadataka prema prioritetima i vremenskim zahtjevima.

## Kompajliranje

Program zahtjeva minimalno C++17 kompajler i pthread biblioteku.

```bash
# Kompajliranje pomoću Makefile
make

# Alternativno, direktno kompajliranje
clang++ -std=c++17 -pthread -o scheduler main.cpp task_scheduler.cpp thread_pool.cpp
```

## Pokretanje

```bash
# Pokretanje sa defaultnim brojem niti (3)
./scheduler

# Pokretanje sa specifičnim brojem niti
./scheduler 6
./scheduler 1
```

## Argumenti

Program prima opcionalni argument:
- `broj_niti` - broj radnih niti u thread pool-u (default: 3)

### Primjeri:
```bash
./scheduler          # Koristi 3 radne niti
./scheduler 4        # Koristi 4 radne niti
./scheduler 10       # Koristi 10 radnih niti
```

## Struktura projekta

```
.
├── main.cpp            # Glavna funkcija i demonstracija
├── task.h              # Definicija Task strukture
├── thread_pool.h       # Header za ThreadPool klasu
├── thread_pool.cpp     # Implementacija ThreadPool klase
├── task_scheduler.h    # Header za TaskScheduler klasu
├── task_scheduler.cpp  # Implementacija TaskScheduler klase
├── Makefile           # Build skripta
└── README.md          # Ovaj fajl
```


## Demonstracija

Program demonstrira izvršavanje 4 zadatka:
- Zadatak 1: start=1000ms, duration=3000ms, priority=2
- Zadatak 2: start=1000ms, duration=1000ms, priority=1
- Zadatak 3: start=1500ms, duration=2000ms, priority=3
- Zadatak 4: start=1500ms, duration=1000ms, priority=1

Očekivani redoslijed izvršavanja: 2 → 1 → 4 → 3

## Čišćenje

```bash
make clean
```
