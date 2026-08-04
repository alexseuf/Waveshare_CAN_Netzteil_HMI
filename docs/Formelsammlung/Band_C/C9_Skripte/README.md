# C9-Skripte – reproduzierbare Diagrammerzeugung

Dieses Verzeichnis enthält parametrierbare Python-Referenzimplementierungen zur Neuerzeugung der Diagramme einer PFC-Drossel-Dokumentation.

## Dateien

- `parameter.py`: zentraler Parametersatz für die allgemeine Diagrammerzeugung.
- `generate_all_figures.py`: Erzeugung der allgemeinen mechanischen, magnetischen, elektrischen und thermischen Diagramme.
- `dpwmmin_variable_ldiff.py`: schaltzustandsbasierte Berechnung der Stromwelligkeit von Phase A bei Minimum-Clamp-SVM.
- `ldiff_kennlinie_template.csv`: Vorlage für eine gemessene oder digitalisierte differentielle Induktivitätskennlinie.

## Installation

```bash
python -m pip install numpy matplotlib
```

## Allgemeine Diagrammerzeugung

```bash
python generate_all_figures.py
```

Die Ausgabe wird standardmäßig im Unterverzeichnis `generated` abgelegt.

## DPWMmin-Stromwelligkeit

Berechnung mit der analytischen Tanh-Entwurfskennlinie:

```bash
python dpwmmin_variable_ldiff.py \
  --output generated/dpwmmin_delta_ipp_phase_a.svg
```

Berechnung mit einer gemessenen oder digitalisierten Kennlinie:

```bash
python dpwmmin_variable_ldiff.py \
  --ldiff-csv ldiff_kennlinie_template.csv \
  --output generated/dpwmmin_delta_ipp_phase_a.svg
```

Die CSV muss die Spalte `current_a` und entweder `ldiff_h` oder `ldiff_uh` enthalten. Die Kennlinie wird über den Betrag des momentanen Phasenstroms interpoliert.

## DPWMmin-Modell

Das Skript:

1. erzeugt die drei sinusförmigen Phasenreferenzen,
2. addiert die DPWMmin-Nullsystemkomponente,
3. klemmt den kleinsten Referenzwert auf den negativen Zwischenkreis,
4. vergleicht die Tastverhältnisse mit einem symmetrischen Dreiecksträger,
5. berechnet die auf den schwebenden Sternpunkt bezogenen Phasenspannungen,
6. integriert die Ripple-Spannung innerhalb jeder PWM-Periode mit dem lokalen $L_{diff}$,
7. bestimmt $\Delta I_{pp}$ für Leerlauf, 20 kW und 40 kW.

Bei 50 Hz und 70 kHz werden 1400 PWM-Perioden pro Netzperiode ausgewertet. Jede PWM-Periode wird standardmäßig mit 240 Zeitschritten aufgelöst.

## Automatische Prüfungen

Die Skripte prüfen unter anderem:

- positive Geometrie und Windungszahl,
- streng monotone Stromstützstellen,
- positive differentielle Induktivitäten,
- gültige CSV-Spalten,
- Tastverhältnisse im Bereich 0 bis 1.

## Modellgrenzen

Die Skripte ersetzen keine abschließende PLECS-Schaltsimulation, FEM-Feldberechnung oder Messung. Totzeiten, Halbleiterspannungsabfälle, Reglerkorrekturen und die Änderung von $L_{diff}$ durch den Ripple innerhalb einer PWM-Periode sind im DPWMmin-Referenzmodell nicht enthalten.
