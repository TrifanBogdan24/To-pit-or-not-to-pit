#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#define ZERO 0
#define UNU 1
#define DOI 2
#define TREI 3
#define OPT 8
#define ZECE 10
#define DOUA_ZECI 20
#define SUTA 100
#define MIE 1000
#define MIN_PRESSURE 19
#define MAX_PRESURE 28
#define MAX_TEMP 120
#define PRINT "print"
#define ANALYSE "analyze"
#define CLEAR "clear"
#define EXIT "exit"


int verify(sensor senzor)
{
	// subprogramul intoarce 1 daca valorile sunt in parametri corecti
	// subprogramul intoarce 0 daca senzorul indica eronat
	if (senzor.sensor_type == TIRE) {
		tire_sensor *tire_data = (tire_sensor *)(senzor.sensor_data);
		if (tire_data->pressure < MIN_PRESSURE)
			return ZERO;
		if (tire_data->pressure > MAX_PRESURE)
			return ZERO;
		if (tire_data->temperature < ZERO)
			return ZERO;
		if (tire_data->temperature > MAX_TEMP)
			return ZERO;
		if (tire_data->wear_level < ZERO)
			return ZERO;
		if (tire_data->wear_level > SUTA)
			return ZERO;
		return UNU;
	}
	if (senzor.sensor_type == PMU) {
		power_management_unit *pmu_data =
			(power_management_unit *)(senzor.sensor_data);
		if (pmu_data->voltage < ZECE)
			return ZERO;
		if (pmu_data->voltage > DOUA_ZECI)
			return ZERO;
		if (pmu_data->current < -SUTA)
			return ZERO;
		if (pmu_data->current > SUTA)
			return ZERO;
		if (pmu_data->power_consumption < ZERO)
			return ZERO;
		if (pmu_data->power_consumption > MIE)
			return ZERO;
		if (pmu_data->energy_regen < ZERO)
			return ZERO;
		if (pmu_data->energy_regen > SUTA)
			return ZERO;
		if (pmu_data->energy_storage < ZERO)
			return ZERO;
		if (pmu_data->energy_storage > SUTA)
			return ZERO;
		return UNU;
	}
	return UNU;
}


void status_senzor(sensor senzor)
{
	if (senzor.sensor_type == PMU) {
		// senzorul care ni se cere sa il afisam este POWER MANAGEMENT UNIT

		/* Fromat de afisare PMU :
		Power Management Unit
		Voltage: <voltage>
		Current: <current>
		Power Consumption: <power_consumption>
		Energy Regen: <energy_regen>
		Energy Storage: <energy_storage>
		*/
		power_management_unit *pmu_data =
			(power_management_unit *)(senzor.sensor_data);
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", pmu_data->voltage);
		printf("Current: %.2f\n", pmu_data->current);
		printf("Power Consumption: %.2f\n", pmu_data->power_consumption);
		printf("Energy Regen: %d%%\n", pmu_data->energy_regen);
		printf("Energy Storage: %d%%\n", pmu_data->energy_storage);
		return;
	} else if (senzor.sensor_type == TIRE) {
		// senzorul care ni se cere sa il afisam este TIRE

		/* Format de afisare TIRE :
		Tire Sensor
		Pressure: <pressure>
		Temperature: <temperature>
		Wear Level: <wear_level>
		Performance Score: <computed score>/Performance Score: Not Calculated
		*/

		tire_sensor *tire_data = (tire_sensor *)(senzor.sensor_data);
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", tire_data->pressure);
		printf("Temperature: %.2f\n", tire_data->temperature);
		printf("Wear Level: %d%%\n", tire_data->wear_level);
		// printf("Performance Score: %d\n", tire_data->performace_score);
		if (tire_data->performace_score == ZERO)
			printf("Performance Score: Not Calculated\n");
		else
			printf("Performance Score: %d\n",
				tire_data->performace_score);
	}
}

int main(int argc, char const *argv[])
{
	if (argv[UNU] == NULL) {
		printf("Eroare la deschiderea fisierului.");
		return UNU;
	}
	FILE * fin = fopen(argv[UNU], "rb");

	int nr_total_senzori ;
	fread(&nr_total_senzori, sizeof(int), UNU, fin);
	// printf("%d\n" ,nr);

	sensor *senzor = (sensor *) malloc(nr_total_senzori * sizeof(sensor));
	for (int i = ZERO ; i < nr_total_senzori; i++) {
		int tip = ZERO;
		fread(&tip, sizeof(int), UNU, fin);
		// senzor[ZERO]->senzor_type = tip;
		if (tip == UNU) {
			// senzor de tip PMU

			senzor[i].sensor_type = PMU;
			// printf("%d PMU\n", senzor[i].sensor_type);
			senzor[i].sensor_data =
				(void *) malloc(sizeof(power_management_unit));
			power_management_unit *pmu_data =
				(power_management_unit *)(senzor[i].sensor_data);
			fread(&pmu_data->voltage, sizeof(float), UNU, fin);
			fread(&pmu_data->current, sizeof(float), UNU, fin);
			fread(&pmu_data->power_consumption, sizeof(float), UNU, fin);
			fread(&pmu_data->energy_regen, sizeof(int), UNU, fin);
			fread(&pmu_data->energy_storage, sizeof(int), UNU, fin);

			// printf("%.2f %.2f %.2f %d %d\n",
			// pmu_data->voltage, pmu_data->current,
			// pmu_data->power_consumption,
			// pmu_data->energy_regen,
			// pmu_data->energy_storage);

		} else if (tip == ZERO) {
			// senzor de tip Tire

			senzor[i].sensor_type = TIRE;
			// printf("%d TIRE\n", senzor[i].sensor_type);
			senzor[i].sensor_data = (void *) malloc(sizeof(tire_sensor));
			tire_sensor *tire_data = (tire_sensor *) senzor[i].sensor_data;
			fread(&tire_data->pressure, sizeof(float), UNU, fin);
			fread(&tire_data->temperature, sizeof(float), UNU, fin);
			fread(&tire_data->wear_level, sizeof(int), UNU, fin);
			fread(&tire_data->performace_score, sizeof(int), UNU, fin);
			// printf("%.2f %.2f %d %d\n",
			// tire_data->pressure,
			// tire_data->temperature,
			// tire_data->wear_level,
			// tire_data->performace_score);

		}

		fread(&senzor[i].nr_operations, sizeof(int), UNU, fin);
		// printf("%d\n", senzor[i].nr_operations);
		senzor[i].operations_idxs =
			(int *) malloc(senzor[i].nr_operations * sizeof(int));
		for (int j = ZERO; j < senzor[i].nr_operations; j++) {
			fread(&senzor[i].operations_idxs[j], sizeof(int), UNU, fin);
			// printf("%d ", senzor[i].operations_idxs[j]);
		}
		// printf("\n");
	}


	// keep it simple ;)
	sensor *senzor_auxiliar =
		(sensor *) malloc(nr_total_senzori * sizeof(sensor));
	int parcurgere = ZERO;
	for (int i = ZERO; i < nr_total_senzori; i++) {
		if (senzor[i].sensor_type == PMU) {
			senzor_auxiliar[parcurgere++] = senzor[i];
		}
	}
	for (int i = ZERO; i < nr_total_senzori; i++) {
		if (senzor[i].sensor_type == TIRE) {
			senzor_auxiliar[parcurgere++] = senzor[i];
		}
	}
	for (int i = ZERO; i < nr_total_senzori; i++) {
		senzor[i] = senzor_auxiliar[i];
	}
	// for(int i = ZERO; i <= nr_total_senzori-1; i++) {
	// 	if (senzor[i].sensor_type == PMU) printf("PMU\n");
	// 	else if (senzor[i].sensor_type == TIRE) printf("TIRE\n");
	// }

	char *command = (char *) malloc(ZECE * sizeof(char));
	int ok = UNU;
	while (ok == UNU) {
		scanf("%s", command);
		if (strstr(command, PRINT) != NULL) {
			int indice_senzor = ZERO;
			scanf("%d", &indice_senzor);
			// printf("\n %d \n ", indice_senzor);
			if (ZERO <= indice_senzor &&
				indice_senzor <= nr_total_senzori - UNU) {
				status_senzor(senzor[indice_senzor]);
			} else {
				printf("Index not in range!\n");
			}
		} else if (strstr(command, ANALYSE) != NULL) {
			int indice_senzor = ZERO;
			scanf("%d", &indice_senzor);
			//printf("\n %d \n ", indice_senzor);
			if (ZERO <= indice_senzor &&
				indice_senzor <= nr_total_senzori - UNU) {
				void *functie[OPT];
				get_operations(functie);
				for (int i = ZERO; i <
					senzor[indice_senzor].nr_operations; i++) {
					// apelul functiei :
					((void (*)()) functie[senzor[indice_senzor]
						.operations_idxs[i]]) (senzor[indice_senzor]
						.sensor_data);
						// echivalent cu f();
				}
			} else {
				printf("Index not in range!\n");
			}
		} else if (strstr(command, EXIT) != NULL) {
			ok = ZERO;
			for (int i = ZERO; i < nr_total_senzori; i++) {
				free(senzor[i].sensor_data);
				free(senzor[i].operations_idxs);
			}
			free(senzor_auxiliar);
			free(senzor);
		} else if (strstr(command, CLEAR) != NULL) {
			/*
			vom muta toti senzorii care contin valori eronate
				in stanga 'vectorului lui pointeri'
			vom afla numarul de senzori cu valori malitioase,
				pe care il notam cu nr_to_be_deleted
			vom sterge din vectorul de senzori,
				ultimi nr_to_be_deleted senzori din vectori
			*/
			int i = ZERO;
			while (i < nr_total_senzori) {
				// verificam daca au ramas doar senzorii care dau valori corecte
				if (verify(senzor[i]) == ZERO) {
					// senzorul de pe pozitia i indica eronat
					// dorim sa stergem, deci, senzorul de pe pozitia i
					sensor aux = senzor[i];
					for (int j = i; j <  nr_total_senzori - UNU; j++) {
						senzor[j] = senzor[j + UNU];
					}
					senzor[nr_total_senzori-1] = aux;
					free(senzor[nr_total_senzori - UNU].sensor_data);
					free(senzor[nr_total_senzori - UNU].operations_idxs);
					nr_total_senzori--;
				} else {
					i++;
				}
			}
		}
	}
	free(command);
	fclose(fin);
	return 0;
}
