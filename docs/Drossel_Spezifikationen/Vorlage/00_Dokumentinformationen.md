---
title: "Entwicklungsspezifikation"
subtitle: "<Projektname>"
document: "Drosselspezifikation"
version: "0.1"
status: "Vorlage"
author: "<Autor>"
date: "<Datum>"
---

# Entwicklungsspezifikation

## <Projektname>

> Dokumentvorlage zur Entwicklung, Berechnung, Fertigung und Freigabe magnetischer Drosseln

---

# Dokumentinformationen

| Parameter | Wert |
|---|---|
| Dokumentnummer | |
| Dokumentversion | |
| Status | Entwurf / Review / Freigabe |
| Projekt | |
| Kunde | |
| Baugruppe | |
| Autor | |
| Prüfer | |
| Freigeber | |
| Erstellungsdatum | |
| Letzte Änderung | |

---

# Änderungsverlauf

| Version | Datum | Autor | Beschreibung |
|---|---|---|---|
| 0.1 | | | Dokument erstellt |
| | | | |
| | | | |

---

# Inhaltsverzeichnis

> Das Inhaltsverzeichnis wird von GitHub automatisch aus den Überschriften erzeugt.

---

# 1 Zweck und Geltungsbereich

## 1.1 Zielsetzung

Diese Spezifikation beschreibt den vollständigen Entwicklungsstand einer magnetischen Drossel einschließlich

- elektrischer Auslegung,
- magnetischer Berechnung,
- mechanischem Aufbau,
- thermischer Auslegung,
- Fertigungsanforderungen sowie
- Prüf- und Freigabekriterien.

Das Dokument dient als gemeinsame technische Grundlage für Entwicklung, Simulation, Konstruktion, Fertigung und Qualifikation.

## 1.2 Geltungsbereich

Diese Spezifikation gilt für folgende Baugruppe.

| Parameter | Wert |
|---|---|
| Projekt | |
| Baugruppe | |
| Topologie | |
| Anwendung | |
| Hardware-Version | |
| Dokumentstatus | |

## 1.3 Zielvorgaben

Die Entwicklung verfolgt folgende Ziele.

| Ziel | Sollwert |
|---|---|
| Induktivität | |
| Nennstrom | |
| Spitzenstrom | |
| Schaltfrequenz | |
| Kernmaterial | |
| Wirkungsgrad | |
| Temperaturklasse | |
| Lebensdauer | |

## 1.4 Mitgeltende Dokumente

| Dokument | Version | Bemerkung |
|---|---|---|
| Formelsammlung Drosselauslegung | aktuell | Berechnungsgrundlagen |
| Kern-Datenblatt | | Hersteller |
| Wicklungszeichnung | | |
| Stückliste | | |
| CAD-Modell | | |
| Prüfplan | | |

## 1.5 Normen

| Norm | Beschreibung |
|---|---|
| IEC 61558 | Transformatoren und Drosseln |
| IEC 60076 | Leistungstransformatoren und Drosseln |
| IEC 61800 | Elektrische Leistungsantriebssysteme |
| IEC 62368 | Sicherheitsanforderungen |
| UL 94 | Brennbarkeit von Kunststoffen |
| Projektspezifische Normen | |

## 1.6 Begriffe und Abkürzungen

| Abkürzung | Beschreibung |
|---|---|
| PFC | Power Factor Correction |
| AFE | Active Front End |
| RMS | Effektivwert |
| DC | Gleichstrom |
| AC | Wechselstrom |
| Ae | Effektiver Kernquerschnitt |
| le | Effektive magnetische Weglänge |
| Ve | Effektives Kernvolumen |
| B | Flussdichte |
| H | Feldstärke |
| Ldiff | Differentielle Induktivität |
| Lsec | Sekanteninduktivität |
| RDC | Gleichstromwiderstand |
| RAC | Wechselstromwiderstand |
| MSE | Modified Steinmetz Equation |
| iGSE | Improved Generalized Steinmetz Equation |

## 1.7 Dokumentaufbau

| Kapitel | Inhalt |
|---:|---|
| 2 | Systemanforderungen |
| 3 | Magnetischer Aufbau |
| 4 | Wicklungsaufbau |
| 5 | Mechanischer Aufbau |
| 6 | Berechnungsgrundlagen |
| 7 | Magnetische Kennlinien |
| 8 | Stromrippel und Flussdichtehub |
| 9 | Kupferverluste |
| 10 | Kernverluste |
| 11 | Gesamtverluste |
| 12 | Thermische Auslegung |
| 13 | Fertigungsanforderungen |
| 14 | Prüf- und Abnahmekriterien |
| 15 | Simulationsparameter (PLECS / MATLAB) |
| 16 | Bewertung und offene Punkte |
| 17 | Quellen |

## 1.8 Dokumentpflege

Diese Spezifikation ist ein lebendes Entwicklungsdokument.

Änderungen an

- Kernmaterial,
- Wicklung,
- Simulationsmodell,
- thermischer Auslegung,
- Fertigungsprozess oder
- Prüfkriterien

sind im Änderungsverlauf zu dokumentieren und durch eine neue Dokumentversion freizugeben.
