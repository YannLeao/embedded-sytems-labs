#!/usr/bin/env bash
# Optional local check with the XC8 installation used by the original course.
set -euo pipefail

project_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
xc8_compiler="${XC8_CC:-/opt/microchip/xc8/v3.10/bin/xc8-cc}"
atmega_pack="${ATMEGA_DFP:-/opt/microchip/mplabx/v6.35/packs/Microchip/ATmega_DFP/3.6.299}"
output_dir="${1:-$project_root/build/xc8-check}"

if [[ ! -x "$xc8_compiler" || ! -d "$atmega_pack/xc8" ]]; then
    printf 'Set XC8_CC to xc8-cc and ATMEGA_DFP to the ATmega device-pack directory.\n' >&2
    exit 1
fi

mkdir -p -- "$output_dir"
for source_path in "$project_root"/labs/*/main.c; do
    lab_name="$(basename -- "$(dirname -- "$source_path")")"
    printf 'Building %s for ATmega2560\n' "$lab_name"
    "$xc8_compiler" -mcpu=ATmega2560 -mdfp="$atmega_pack/xc8" \
        -O1 -Wall -Wextra -Wl,-Map="$output_dir/$lab_name.map" \
        -o "$output_dir/$lab_name.elf" "$source_path"
done
