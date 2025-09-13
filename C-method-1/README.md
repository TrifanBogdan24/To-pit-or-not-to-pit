# 🏎️ To pit or not pit...

A **data management** CLI app, built in **C**
to analyze sensors values of a Ferrari **Formula 1** car.

> Details of implementation for *Method 1* (written in C).

## Sorting Sensors

When sorting sensors from pointers,
I took into account the fact that order (in which sensors of the same type appear) must be kept.

A sorting method, which I considered to be more appropriate to me and easy to implement,
consists of taking an auxiliary pointer of sensors,
initially empty and **iterating** the sensors (from left to right) **twice**:
1. During the first iteration: I added only the **PMU** sensors to the auxiliary pointer
2. In the second: only those who measure **Tire** parameters/

At the end, I replaced (each element of) the initial pointer(s) with the auxiliary one.

Thus, sorting has a time complexity of `O(2 * N) = O(N)`
where **N** is the total number of sensors.


> 💡 Another (*perhaps more efficient method*) would have been 
> to build two **linked lists** as we read the sensors:
> 1. 🔗 one for **PMU** sensors
> 2. 🔗 another one for **Tire** values
>
> Concattenating the lists would result in an array, sorted by sensor type.


> 🎯 Even though more complex methods may exist,
> the one that I choose aligns better with the goals and context of my implementation.



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

