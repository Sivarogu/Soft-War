#pragma once

typedef struct s_energy_cell {
	size_t x;
	size_t y;
	size_t energy;
	struct s_energy_cell *next;
} t_energy_cell;

int energy_cell_new(size_t x, size_t y, size_t energy);
void energy_cell_destroy(t_energy_cell * energy_cell);
