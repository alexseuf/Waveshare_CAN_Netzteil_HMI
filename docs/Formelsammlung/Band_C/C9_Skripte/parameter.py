"""Zentraler Parametersatz für die reproduzierbare Diagrammerzeugung.

Alle Größen werden in SI-Einheiten angegeben, sofern der Variablenname
keine andere Einheit enthält. Die Datei kann für neue Drosselvarianten
kopiert und angepasst werden.
"""

from dataclasses import dataclass, field
import numpy as np


@dataclass
class ChokeParameters:
    # Kerngeometrie
    core_name: str = "Magnetics C058110A2"
    material_name: str = "High Flux 60 µ"
    outer_diameter_m: float = 57.15e-3
    inner_diameter_m: float = 35.56e-3
    core_height_m: float = 13.97e-3
    core_count: int = 3
    effective_area_m2: float = 3 * 144e-6
    effective_length_m: float = 143e-3
    mu_r0: float = 60.0
    b_sat_t: float = 1.78

    # Wicklung
    turns: int = 48
    turns_layer_1: int = 28
    turns_layer_2: int = 20
    strand_count: int = 630
    strand_diameter_m: float = 0.10e-3
    winding_resistance_25_ohm: float = 23.2e-3
    copper_temperature_coefficient_per_k: float = 0.00393
    ac_resistance_factor: float = 1.0

    # Elektrisches System
    line_voltage_rms_v: float = 400.0
    dc_link_voltage_v: float = 750.0
    mains_frequency_hz: float = 50.0
    switching_frequency_hz: float = 70e3
    load_currents_rms_a: tuple[float, float] = (28.87, 57.74)
    load_labels: tuple[str, str] = ("20 kW Dauer", "40 kW Spitze")

    # Korrigierte Ldiff-Stützstellen
    current_support_a: np.ndarray = field(default_factory=lambda: np.array(
        [0.0, 28.9, 40.8, 57.7, 81.6, 90.0], dtype=float
    ))
    ldiff_support_h: np.ndarray = field(default_factory=lambda: np.array(
        [525.0, 364.0, 280.0, 202.0, 135.0, 119.0], dtype=float
    ) * 1e-6)

    # B(I)-Stützstellen aus dem dokumentierten Beispiel
    b_current_support_a: np.ndarray = field(default_factory=lambda: np.array(
        [0.0, 28.9, 40.8, 57.7, 81.6, 90.0], dtype=float
    ))
    b_support_t: np.ndarray = field(default_factory=lambda: np.array(
        [0.0, 0.663, 0.849, 1.038, 1.230, 1.281], dtype=float
    ))

    # Steinmetz: Pv[mW/cm³] = a * B[T]^b * f[kHz]^c
    steinmetz_a: float = 246.54
    steinmetz_b: float = 2.218
    steinmetz_c: float = 1.311

    # Thermik
    thermal_resistance_k_per_w: float = 3.11
    thermal_capacity_j_per_k: float = 800.0
    overload_duration_s: float = 0.5
    ambient_temperature_c: float = 25.0
    hot_winding_temperature_c: float = 120.0

    # Numerik
    points_per_switching_period: int = 100
    current_plot_max_a: float = 90.0
    current_plot_points: int = 1201
    mains_angle_points: int = 1441


PARAMS = ChokeParameters()
