# Game Project

Prototyp gry w **Unreal Engine 5.6** z własnym, lekkim systemem zdolności, efektów statusowych i statystyk opartym na **Gameplay Tags** oraz **Data Assets**.

Gracz steruje postacią z perspektywy trzeciej osoby, może sprintować, przełączać aktywną zdolność oraz używać umiejętności ofensywnych. Stan zdrowia wyświetlany jest na pasku HP nad postacią.

## Spis treści

* Informacje techniczne
* Najważniejsze decyzje techniczne
* Ograniczenia
* Sterowanie

## Informacje techniczne

* **Silnik:** Unreal Engine 5.6
* **IDE:** Visual Studio 2022

### Uruchomienie

1. Otwórz `GameProject.uproject`.
2. W razie potrzeby potwierdź wygenerowanie plików projektu i kompilację modułów.
3. Domyślnie zostanie załadowana mapa `Content/Maps/M_MainMap.umap`.

## Najważniejsze decyzje techniczne

### Własny system zdolności

Zamiast Gameplay Ability System (GAS) zastosowano własny system oparty o trzy komponenty:

* `UStatsComponent` – zarządzanie statystykami,
* `UAbilityComponent` – obsługa zdolności,
* `UStatusEffectComponent` – zarządzanie efektami statusowymi.

Rozwiązanie jest prostsze, w pełni napisane w C++ i łatwe do rozbudowy.

### Zdolności jako Data Assets

Zdolności są definiowane jako `UDataAsset`, dzięki czemu dodawanie nowych umiejętności nie wymaga zmian w kodzie C++.

### Gameplay Tags

Statystyki, zdolności oraz warunki ich użycia identyfikowane są za pomocą `FGameplayTag`, co pozwala łatwo rozszerzać system bez rekompilacji projektu.

### UI i Input

* Pasek zdrowia aktualizowany jest reaktywnie na podstawie delegatów z `UStatsComponent`.
* Sterowanie wykorzystuje **Enhanced Input**.

## Ograniczenia

* Brak AI przeciwników.
* Brak obsługi multiplayera.
* Brak zapisu i odczytu stanu gry.
* Akcje **Jump** i **Interact** są zbindowane, ale niezaimplementowane.

## Sterowanie

| Akcja                     | Klawisz                        |
| ------------------------- | ------------------------------ |
| Ruch                      | WASD                           |
| Rozglądanie               | Mysz                           |
| Sprint                    | Shift                          |
| Użycie zdolności          | LPM                            |
| Zmiana aktywnej zdolności | PPM                            |
| Skok                      | Spacja *(niezaimplementowany)* |
| Interakcja                | E *(niezaimplementowana)*      |

Po uruchomieniu gracz posiada dwie przykładowe zdolności: pocisk (`FireBlast`) oraz efekt obszarowy (`FireRing`).
