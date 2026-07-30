"""Exakte Leiterpositionen einer zweilagigen Ringkernwicklung.

Erzeugt:
- leiterpositionen_6_kreise.csv
- leiterpositionen_6_kreise.svg
- leiterpositionen_6_kreise.png

Alle Längen werden intern in Millimetern berechnet.
"""

from __future__ import annotations

import argparse
import csv
import math
from dataclasses import dataclass
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.patches import Circle, Wedge


@dataclass(frozen=True)
class Geometry:
    d_core_inner_mm: float = 35.56
    d_core_outer_coated_mm: float = 59.20
    strand_count: int = 630
    strand_diameter_mm: float = 0.10
    packing_factor: float = 0.68

    @property
    def litz_diameter_mm(self) -> float:
        return self.strand_diameter_mm * math.sqrt(
            self.strand_count / self.packing_factor
        )

    @property
    def r3_mm(self) -> float:
        return self.d_core_inner_mm / 2

    @property
    def r4_mm(self) -> float:
        return self.d_core_outer_coated_mm / 2

    @property
    def r2_mm(self) -> float:
        return self.r3_mm - self.litz_diameter_mm / 2

    @property
    def r5_mm(self) -> float:
        return self.r4_mm + self.litz_diameter_mm / 2

    @property
    def r1_mm(self) -> float:
        return self.r2_mm - self.litz_diameter_mm

    @property
    def r6_mm(self) -> float:
        return self.r5_mm + self.litz_diameter_mm


@dataclass(frozen=True)
class Conductor:
    global_number: int
    circle_number: int
    conductor_on_circle: int
    conductors_on_circle: int
    angle_deg: float
    radius_mm: float
    x_mm: float
    y_mm: float


def calculate(g: Geometry) -> list[Conductor]:
    definitions = (
        (1, 20, g.r1_mm),
        (2, 28, g.r2_mm),
        (5, 28, g.r5_mm),
        (6, 20, g.r6_mm),
    )

    result: list[Conductor] = []
    global_number = 1
    for circle_number, count, radius_mm in definitions:
        for j in range(count):
            angle_rad = 2 * math.pi * j / count
            result.append(
                Conductor(
                    global_number=global_number,
                    circle_number=circle_number,
                    conductor_on_circle=j + 1,
                    conductors_on_circle=count,
                    angle_deg=math.degrees(angle_rad),
                    radius_mm=radius_mm,
                    x_mm=radius_mm * math.cos(angle_rad),
                    y_mm=radius_mm * math.sin(angle_rad),
                )
            )
            global_number += 1
    return result


def export_csv(conductors: list[Conductor], out: Path) -> None:
    with out.open("w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file, delimiter=";")
        writer.writerow(
            [
                "Gesamt_Nr",
                "Kreis",
                "Leiter_auf_Kreis",
                "Leiterzahl_Kreis",
                "Winkel_deg",
                "Radius_mm",
                "Kreisdurchmesser_mm",
                "x_mm",
                "y_mm",
            ]
        )
        for c in conductors:
            writer.writerow(
                [
                    c.global_number,
                    c.circle_number,
                    c.conductor_on_circle,
                    c.conductors_on_circle,
                    f"{c.angle_deg:.9f}",
                    f"{c.radius_mm:.6f}",
                    f"{2*c.radius_mm:.6f}",
                    f"{c.x_mm:.6f}",
                    f"{c.y_mm:.6f}",
                ]
            )


def draw(g: Geometry, conductors: list[Conductor], out: Path) -> None:
    colors = {1: "tab:blue", 2: "tab:green", 5: "tab:orange", 6: "tab:purple"}
    radii = {1: g.r1_mm, 2: g.r2_mm, 5: g.r5_mm, 6: g.r6_mm}

    fig, ax = plt.subplots(figsize=(10, 10))
    ax.add_patch(
        Wedge(
            (0, 0),
            g.r4_mm,
            0,
            360,
            width=g.r4_mm - g.r3_mm,
            facecolor="none",
            edgecolor="black",
            hatch="///",
            linewidth=1.2,
        )
    )

    ax.add_patch(Circle((0, 0), g.r3_mm, fill=False, color="black", linewidth=1.4))
    ax.add_patch(Circle((0, 0), g.r4_mm, fill=False, color="black", linewidth=1.4))

    for circle_number, radius_mm in radii.items():
        ax.add_patch(
            Circle(
                (0, 0),
                radius_mm,
                fill=False,
                color=colors[circle_number],
                linestyle="--",
                linewidth=0.9,
            )
        )

    conductor_radius = g.litz_diameter_mm / 2
    for c in conductors:
        ax.add_patch(
            Circle(
                (c.x_mm, c.y_mm),
                conductor_radius,
                fill=False,
                color=colors[c.circle_number],
                linewidth=1.1,
            )
        )

    ax.axhline(0, color="black", linewidth=0.5)
    ax.axvline(0, color="black", linewidth=0.5)
    ax.set_aspect("equal", adjustable="box")
    limit = g.r6_mm + conductor_radius + 4
    ax.set_xlim(-limit, limit)
    ax.set_ylim(-limit, limit)
    ax.set_xlabel("x [mm]")
    ax.set_ylabel("y [mm]")
    ax.set_title(
        "Maßstäbliche Leiteranordnung\n"
        f"d_Litze = {g.litz_diameter_mm:.3f} mm"
    )
    ax.grid(True, linewidth=0.35)
    fig.tight_layout()
    fig.savefig(out.with_suffix(".svg"), bbox_inches="tight")
    fig.savefig(out.with_suffix(".png"), dpi=300, bbox_inches="tight")
    plt.close(fig)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=Path("generated"))
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)

    geometry = Geometry()
    conductors = calculate(geometry)
    export_csv(conductors, args.output / "leiterpositionen_6_kreise.csv")
    draw(geometry, conductors, args.output / "leiterpositionen_6_kreise")

    print(f"d_Litze = {geometry.litz_diameter_mm:.3f} mm")
    print(f"D1 = {2*geometry.r1_mm:.3f} mm")
    print(f"D2 = {2*geometry.r2_mm:.3f} mm")
    print(f"D3 = {2*geometry.r3_mm:.3f} mm")
    print(f"D4 = {2*geometry.r4_mm:.3f} mm")
    print(f"D5 = {2*geometry.r5_mm:.3f} mm")
    print(f"D6 = {2*geometry.r6_mm:.3f} mm")
    print(f"Leiterpositionen = {len(conductors)}")


if __name__ == "__main__":
    main()
