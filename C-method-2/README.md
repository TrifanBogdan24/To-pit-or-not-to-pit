# 🏎️ To pit or not pit...

A **data management** CLI app, built in **C**
to analyze sensors values of a Ferrari **Formula 1** car.

> Details of implementation for *Method 2* (written in C).


## 🔗 Sorting Sensors using **Linked Lists**


Sorting the array of sensors by type takes place while reading the input file.

Let me walk you through.

To store sensors and group them by type, I used **two linked lists**
1. 🔗 one for **PMU** sensors
2. 🔗 the second one for **Tire** sensors


```c
typedef struct node {
  sensor sensor;
  struct node *next;
} ListNode;
```

> 🎯 With this **data structure**,
> I achieved to preserve the original file order for each sensor type.

Keeping a **pointer to the end of each list** (tail pointer)
**makes append operations faster**,
in just `O(1)`, without having to iterate the list before adding a new sensor.


Let's take a look at **PMU** sensor for example:
```c
// Read PMU sensor data from file
sensor sensor = fread_PMU_sensor_values(fin);

if (!pmu_sensors_head) {
  // Initialize linked list
  pmu_sensors_head = pmu_sensors_tail = new_list_node(sensor);
} else {
  // Append to the end of list
  pmu_sensors_tail->next = new_list_node(sensor);
  pmu_sensors_tail = pmu_sensors_tail->next;
}
```

After reading, I **concatenated** both lists **into an array**
(deallocating memory as I iterate them).

The resulted array will contain all **PMU** sensors first,
followed by all **Tire** sensors.

Excluding file I/O, sorting runs at a **time complexity** of exact `Θ(N)`.



## Analyze Sensors (Function call)

**Requirement**: The coresponding sensor functions MUST BE called through the array.

> Under no circumstances the functions can be explicitally called.


In order to do so, I've created a **pointer of eight elements**, **each element pointing to a specific function**.

For the actual call of the function,
I made a **pointer casting** to extract the desired function.
The call is made using the `sensor_data` field of the `sensor` data structure.


```c
int idx = 0;     // sensor index
scanf("%d", &idx);
if (0 <= idx && idx <= num_sensors - 1) {
  void *operations[NUM_OPERATIONS];
  get_operations(operations);
  for (int i = 0; i < sensors_array[idx].nr_operations; i++) {
    // operations call :
    ((void (*)()) operations[sensors_array[idx]
      .operations_idxs[i]]) (sensors_array[idx]
      .sensor_data);
      // equivalent to f();
  }
}
```

## ✅🔁 Automated Testing

Curious how everything gets tested automatically?

Take a look at the [tests/](./tests/) folder.

