#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"

#define COMMAND_BUFFER_LEGNTH 10
#define NUM_OPERATIONS        8
#define FULL_PERCENTAGE       100

#define MIN_TIRE_PRESSURE 19
#define MAX_TIRE_PRESURE  28
#define MAX_TIRE_TEMP     120

#define MAX_PMU_POWER_CONSUMPTION 1000
#define MIN_PMU_CURRENT -100
#define MAX_PMU_CURRENT 100
#define MIN_PMU_VOLTAGE 10
#define MAX_PMU_VOLTAGE 20

#define PRINT   "print"
#define ANALYSE "analyze"
#define CLEAR   "clear"
#define EXIT    "exit"


ListNode *new_list_node(sensor sensor)
{
	ListNode *node = (ListNode *) malloc(sizeof(ListNode));
	node->sensor = sensor;
	node->next = NULL;
	return node;
}

/*
* It will return 0 if sensor values are within optimal range
* and -1, otherwise.
*/
int verify_sensor(sensor senzor)
{
	if (senzor.sensor_type == TIRE) {
		tire_sensor *tire_data = (tire_sensor *)(senzor.sensor_data);
		if (tire_data->pressure < MIN_TIRE_PRESSURE)
			return -1;
		if (tire_data->pressure > MAX_TIRE_PRESURE)
			return -1;
		if (tire_data->temperature < 0)
			return -1;
		if (tire_data->temperature > MAX_TIRE_TEMP)
			return -1;
		if (tire_data->wear_level < 0)
			return -1;
		if (tire_data->wear_level > FULL_PERCENTAGE)
			return -1;
		return 0;
	}
	if (senzor.sensor_type == PMU) {
		power_management_unit *pmu_data =
			(power_management_unit *)(senzor.sensor_data);
		if (pmu_data->voltage < MIN_PMU_VOLTAGE)
			return -1;
		if (pmu_data->voltage > MAX_PMU_VOLTAGE)
			return -1;
		if (pmu_data->current < MIN_PMU_CURRENT)
			return -1;
		if (pmu_data->current > MAX_PMU_CURRENT)
			return -1;
		if (pmu_data->power_consumption < 0)
			return -1;
		if (pmu_data->power_consumption > MAX_PMU_POWER_CONSUMPTION)
			return -1;
		if (pmu_data->energy_regen < 0)
			return -1;
		if (pmu_data->energy_regen > FULL_PERCENTAGE)
			return -1;
		if (pmu_data->energy_storage < 0)
			return -1;
		if (pmu_data->energy_storage > FULL_PERCENTAGE)
			return -1;
		return 0;
	}
	return -1;
}


/* Print format for PMU sensor :
	Power Management Unit
	Voltage: <voltage>
	Current: <current>
	Power Consumption: <power_consumption>
	Energy Regen: <energy_regen>
	Energy Storage: <energy_storage>
*/
void print_PMU_sensor_status(power_management_unit *pmu_data)
{
	printf("Power Management Unit\n");
	printf("Voltage: %.2f\n", pmu_data->voltage);
	printf("Current: %.2f\n", pmu_data->current);
	printf("Power Consumption: %.2f\n", pmu_data->power_consumption);
	printf("Energy Regen: %d%%\n", pmu_data->energy_regen);
	printf("Energy Storage: %d%%\n", pmu_data->energy_storage);
}


/* Print format for TIRE sensor :
	Tire Sensor
	Pressure: <pressure>
	Temperature: <temperature>
	Wear Level: <wear_level>
	Performance Score: <computed score>/Performance Score: Not Calculated
*/
void print_TIRE_sensor_status(tire_sensor *tire_data)
{
	printf("Tire Sensor\n");
	printf("Pressure: %.2f\n", tire_data->pressure);
	printf("Temperature: %.2f\n", tire_data->temperature);
	printf("Wear Level: %d%%\n", tire_data->wear_level);
	if (tire_data->performace_score) {
		printf("Performance Score: %d\n", tire_data->performace_score);
	} else {
		printf("Performance Score: Not Calculated\n");
	}
}


void print_status(sensor senzor)
{
	if (senzor.sensor_type == PMU) {
		print_PMU_sensor_status((power_management_unit *)(senzor.sensor_data));
	} else if (senzor.sensor_type == TIRE) {
		print_TIRE_sensor_status((tire_sensor *)(senzor.sensor_data));
	}
}



void fread_sensor_operations(FILE *fin, sensor *sensor)
{

	fread(&sensor->nr_operations, sizeof(int), 1, fin);
	sensor->operations_idxs =
		(int *) malloc(sensor->nr_operations * sizeof(int));
	for (int j = 0; j < sensor->nr_operations; j++) {
		fread(&sensor->operations_idxs[j],
			sizeof(int), 1, fin);
	}
}

sensor fread_PMU_sensor_values(FILE *fin)
{
	sensor sensor;

	sensor.sensor_type = PMU;
	sensor.sensor_data =
		(void *) malloc(sizeof(power_management_unit));
	power_management_unit *pmu_data =
		(power_management_unit *)(sensor.sensor_data);
	fread(&pmu_data->voltage, sizeof(float), 1, fin);
	fread(&pmu_data->current, sizeof(float), 1, fin);
	fread(&pmu_data->power_consumption, sizeof(float), 1, fin);
	fread(&pmu_data->energy_regen, sizeof(int), 1, fin);
	fread(&pmu_data->energy_storage, sizeof(int), 1, fin);

	fread_sensor_operations(fin, &sensor);
	return sensor;
}


sensor fread_tire_sensor_values(FILE *fin)
{
	sensor sensor;
	sensor.sensor_type = TIRE;
	sensor.sensor_data =
		(void *) malloc(sizeof(tire_sensor));
	tire_sensor *tire_data =
		(tire_sensor *) sensor.sensor_data;
	fread(&tire_data->pressure, sizeof(float), 1, fin);
	fread(&tire_data->temperature, sizeof(float), 1, fin);
	fread(&tire_data->wear_level, sizeof(int), 1, fin);
	fread(&tire_data->performace_score, sizeof(int), 1, fin);

	fread_sensor_operations(fin, &sensor);
	return sensor;
}

void fread_sensors_array(char const *file_path,
	int *num_sensors, sensor **sensors_array)
{
	// "rb" -> read binary file
	FILE *fin = fopen(file_path, "rb");

	if (fin == NULL) {
		printf("[ERR] Something went wrong when \
			trying to open the %s file.\n", file_path);
		exit(EXIT_FAILURE);
	}

	// pointers to the beginning of the lists:
	ListNode *pmu_sensors_head  = NULL;
	ListNode *tire_sensors_head = NULL;

	// pointers to the end of the lists:
	ListNode *pmu_sensors_tail = NULL;
	ListNode *tire_sensors_tail = NULL;

	fread(num_sensors, sizeof(int), 1, fin);
	*sensors_array = (sensor *) malloc((*num_sensors) * sizeof(sensor));

	for (int i = 0 ; i < *num_sensors; i++) {
		int tip = 0;
		fread(&tip, sizeof(int), 1, fin);
		if (tip == 1) {
			// Read PMU sensor data from file
			sensor sensor = fread_PMU_sensor_values(fin);

			if (!pmu_sensors_head) {
				pmu_sensors_head = pmu_sensors_tail = new_list_node(sensor);
			} else {
				pmu_sensors_tail->next = new_list_node(sensor);
				pmu_sensors_tail = pmu_sensors_tail->next;
			}

		} else if (tip == 0) {
			// Read TIRE sensor data from file
			sensor sensor = fread_tire_sensor_values(fin);

			if (!tire_sensors_head) {
				tire_sensors_head = tire_sensors_tail = new_list_node(sensor);
			} else {
				tire_sensors_tail->next = new_list_node(sensor);
				tire_sensors_tail = tire_sensors_tail->next;
			}
		}
	}


	// Concatenating linked lists will result in an array,
	//     sorted by sensor priority
	int idx = 0;
	for (ListNode *iter = pmu_sensors_head; iter; ) {
		ListNode *tmp = iter;
		(*sensors_array)[idx++] = tmp->sensor;
		iter = iter->next;
		free(tmp);
	}
	for (ListNode *iter = tire_sensors_head; iter; ) {
		ListNode *tmp = iter;
		(*sensors_array)[idx++] = iter->sensor;
		iter = iter->next;
		free(tmp);
	}

	fclose(fin);
}

/*
* Helper operationstion that I advice to be called after sorting the array
*/
void print_all_sensor_types(int num_sensors, sensor *sensors_array)
{
	for (int i = 0; i <= num_sensors - 1; i++) {
		if (sensors_array[i].sensor_type == PMU) {
			printf("PMU\n");
		} else if (sensors_array[i].sensor_type == TIRE) {
			printf("TIRE\n");
		}
	}
}



void command_print(int num_sensors, sensor *sensors_array)
{
	int idx = 0;     // sensor index
	scanf("%d", &idx);
	if (0 <= idx && idx <= num_sensors - 1) {
		print_status(sensors_array[idx]);
	} else {
		printf("Index not in range!\n");
	}


}


void command_analyze(int num_sensors, sensor *sensors_array)
{

	int idx = 0;     // sensor index
	scanf("%d", &idx);
	if (0 <= idx && idx <= num_sensors - 1) {
		void *operations[NUM_OPERATIONS];
		get_operations(operations);
		for (int i = 0; i <
			sensors_array[idx].nr_operations; i++) {
			// operationstion call :
			((void (*)()) operations[sensors_array[idx]
				.operations_idxs[i]]) (sensors_array[idx]
				.sensor_data);
				// equivalent to f();
		}
	} else {
		printf("Index not in range!\n");
	}
}


void command_clear(int *num_sensors, sensor **sensors_array)
{
	int i = 0;
	while (i < *num_sensors) {
		if (verify_sensor((*sensors_array)[i])) {
			// Sensor i contains erroneous data => delete it from the array
			sensor tmp_sensor = (*sensors_array)[i];
			for (int j = i; j <  *num_sensors - 1; j++) {
				(*sensors_array)[j] = (*sensors_array)[j + 1];
			}
			(*sensors_array)[*num_sensors - 1] = tmp_sensor;
			free((*sensors_array)[*num_sensors - 1].sensor_data);
			free((*sensors_array)[*num_sensors - 1].operations_idxs);
			(*num_sensors)--;

			// Resize the vector after clear
			sensor *tmp_array = (sensor *) realloc(
				*sensors_array,
				(*num_sensors) * sizeof(sensor)
			);

			if (tmp_array != NULL) {
				*sensors_array = tmp_array;
			} else {
				// [ERR] Unable to resize sensor array
			}
		} else {
			i++;
		}
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		printf("[ERR] Invalid number of arguments.\n");
		printf("[INFO] The program expects to receive \
			a single argument in CLI:\n\t \
			the path to the binary file containing sensors data.");
		exit(EXIT_FAILURE);
	}

	int num_sensors;
	sensor *sensors_array;
	fread_sensors_array(argv[1], &num_sensors, &sensors_array);

	char *command = (char *) malloc(COMMAND_BUFFER_LEGNTH * sizeof(char));
	while (1) {
		scanf("%s", command);
		if (strstr(command, PRINT) != NULL) {
			command_print(num_sensors, sensors_array);
		} else if (strstr(command, ANALYSE) != NULL) {
			command_analyze(num_sensors, sensors_array);
		} else if (strstr(command, CLEAR) != NULL) {
			command_clear(&num_sensors, &sensors_array);
		} else if (strstr(command, EXIT) != NULL) {
			break;
		}
	}

	for (int i = 0; i < num_sensors; i++) {
		free(sensors_array[i].sensor_data);
		free(sensors_array[i].operations_idxs);
	}
	free(sensors_array);
	free(command);

	return 0;
}
