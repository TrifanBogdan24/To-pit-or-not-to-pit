# 🏎️ To pit or not pit...

A **memory management** app, designed in **C**
to analyze the sensors of a Ferrari **Formula 1** car.


> ⚠️ Spoiler Alert!
>
> This is **NOT** a **data acquisition** project.

## Story Telling

In this **fictional scenario I created for my project**,
Ferrari engineers needed my software expertise to stay competitive during F1 season.
They discovered that faulty sensors (likely cheap ones from AliExpress 😉)
were feeding incorrect data into their systems, affecting performance decisions.

**My job was clear**:
develop solutions to detect and eliminate bad data,
ensuring the team works with accurate insights.
By doing so, I played a direct role in helping Ferrari chase victory on the F1 track.


## My Implementation


In this scenario, there are 2 types of faulty sensors in a Formula 1 car:
1. **PMU** (Power Management Unit) Sensors
2. **Tire** Sensors

Each sensor type has its priority and associated functions.


### Sensor Priority


The values of **Power Management Unit** sensors
are more important than those received from **Tyre Sensors**.
Thus, the Ferrari team will want the **PMU** values to be the first in the sensor vector.

We receive the following sensors as input:
```
Tire_1 Tire_2 PMU_1 PMU_2 Tire_3 PMU_3 Tire_4 Tire_5 PMU_4
```

The vector will contain the sensors in the following order:
```
PMU_1 PMU_2 PMU_3 PMU_4 Tire_1 Tire_2 Tire_3 Tire_4 Tire_5
```

### Sorting Sensors

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


> 💡 Another (*perhaps more efficient method*) would have been a **priority queue**,
> implemented as a *"binary tree"*:
> 1. **PMU** sensors to be in the left branch
> 2. while the **Tire** values in the right

> 🎯 Even though more complex methods may exist,
> the one that I choose aligns better with the goals and context of my implementation.


### Analyze Sensors (Function call)



**Requirement**: The coresponding sensor functions MUST BE called through the array.

> Under no circumstances the functions can be explicitally called.


In order to do so, I've created a **pointer of eight elements**, **each element pointing to a specific function**.

For the actual call of the function,
I made a **pointer casting** to extract the desired function.
The call is made using the `sensor_data` field of the `sensor` data structure.


```c
scanf("%d", &indice_senzor);
if (ZERO <= indice_senzor && indice_senzor <= nr_total_senzori - UNU) {
    void *functie[OPT];
    get_operations(functie);
    for (int i = ZERO; i < senzor[indice_senzor].nr_operations; i++) {
        // function call (equivalent to f() ) :
        ((void (*)()) functie[senzor[indice_senzor]
            .operations_idxs[i]]) (senzor[indice_senzor]
            .sensor_data);
    }
}
```