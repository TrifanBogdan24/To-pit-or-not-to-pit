# 🏎️ To pit or not pit...

A **data management** CLI app,
built to analyze sensors values of a Ferrari **Formula 1** car.


> ⚠️ Spoiler Alert!
> This is **NOT** a **data acquisition** project.

> Sensors values have already been 'collected' and stored in `*.dat` **binary files**.

## 👨‍💻 Preview


![gif](./preview.gif)


## Story Telling

In this **fictional scenario in which my project takes place**,
Ferrari engineers needed my software expertise to stay competitive during F1 season.
They discovered that faulty sensors (likely cheap ones from AliExpress 😉)
were feeding incorrect data into their systems, affecting performance decisions.

**My job was clear**:
develop solutions to detect and eliminate bad data,
ensuring the team works with accurate insights.
By doing so, I played a direct role in helping Ferrari chase victory on the F1 track.

## Sensor Types

In this scenario, there are 2 types of faulty sensors in a Formula 1 car:
1. **PMU** (Power Management Unit) Sensors
2. **Tire** Sensors

Each sensor type has its priority and associated function calls.

## Sensor Priority


Ferrari’s telemetry prioritizes **PMU** readings over **Tire** readings.
The output array must have **all PMU sensors first**, followed by all Tire sensors.

If we receive the following input (read order):
```
Tire_1 Tire_2 PMU_1 PMU_2 Tire_3 PMU_3 Tire_4 Tire_5 PMU_4
```

The vector will contain the sensors in the following order (sorted by priority):
```
PMU_1 PMU_2 PMU_3 PMU_4 Tire_1 Tire_2 Tire_3 Tire_4 Tire_5
```


## 👨‍💻 Software Commands I Provide to Ferrari Engineers

### 1. Simple Print (`print <index>`)
---

It will the print the data received by the sensor the given index.

In the case of an invalid value, the following error message will appear: `Index not in range!`.


Expected output for **PMU Sensors**:
```
Power Management Unit
Voltage: <voltage>
Current: <current>
Power Consumption: <power_consumption>
Energy Regen: <energy_regen>
Energy Storage: <energy_storage>
```

Expected output for **Tire Sensors**:
```
Tire Sensor
Pressure: <pressure>
Temperature: <temperature>
Wear Level: <wear_level>
Performance Score: <computed score>/Performance Score: Not Calculated
```



### 2. Priority Printing
---

It will show data from all sensors, sorting after their type:
**PMU sensors** are more important than **Tire Sensors**.


### 3. Analyze (`analyze <index>`)
---

It will call the functions associated to the given sensor.

Like **simple print**, the `Index not in range!` will appear if the index is invalid.

### 4. `clear`
---

It will delete mulfunctioning sensors (containing error-prone values) from the array of sensors.


### 5. `exit`
---

Gracefully shut down the program after free-ing the memory.


## Sensor Priority


Ferrari’s telemetry prioritizes **PMU** readings over **Tire** readings.
The output array must have **all PMU sensors first**, followed by all Tire sensors.

If we receive the following input (read order):
```
Tire_1 Tire_2 PMU_1 PMU_2 Tire_3 PMU_3 Tire_4 Tire_5 PMU_4
```

The vector will contain the sensors in the following order (sorted by priority):
```
PMU_1 PMU_2 PMU_3 PMU_4 Tire_1 Tire_2 Tire_3 Tire_4 Tire_5
```


## `>_` User Input

- **Command line argument**: path to the `*.dat` file containing sensors data
  - > Use `sensors_*.dat` from [`tests_data/input/`](./tests_data/input/)
- **Stdandard input**: the *"operations"* to perform on sensors data
  - > Use `commands_*.in` from [`tests_data/input/`](./tests_data/input/)

💡 A more elegant solution to run the executable
would be to use **input redirection** (`<`) to provide commands from a file,
as shown in the examples above.



## Multiple Implementations

Two independent implementations of the same project are provided,
each in its own subdirectory:

- [`C-method-1`](C-method-1)
- [`C-method-2`](C-method-2)


## ▶️ How to run the CLI app written in C


```sh
cd src/
make
./main ../checker/input/<sensor-file>.dat < ../checker/input/<command-file>.in
```

Example:

```sh
./main ../checker/input/sensors_print_easy_1.dat < ../checker/input/commands_print_easy_1.in
```

🧹 Don't forget to remove the object and binary files:
```sh
make clean
```



## C Data Structures for Sensors


Sensor:
```c
typedef struct {
	enum sensor_type sensor_type;
	void *sensor_data;
	int nr_operations;
	int *operations_idxs;
} sensor;
```


**PMU** sensor data:
```c
typedef struct __attribute__((__packed__)) {
	float voltage;
	float current;
	float power_consumption;
	int energy_regen;
	int energy_storage;
} power_management_unit;
```


**Tire** sensor data:
```c
typedef struct __attribute__((__packed__)) {
	float pressure;
	float temperature;
	int wear_level;
	int performace_score;
} tire_sensor;

```


### 🧪 GitHub Actions | CI Pipeline
---

No test suite is complete without **Continous Integration**.

I've set up the automated checker with **GitHub Actions**
to run tests on every push and pull request.

Take a look at the CI workflows here:

| Method name | Workflow file |
| :--- | :--- |
| `C-method-1` | [.github/workflows/CI-tests-method-1.yml](.github/workflows/CI-tests-method-1.yml) |
| `C-method-2` | [.github/workflows/CI-tests-method-2.yml](.github/workflows/CI-tests-method-2.yml) |


### 🌃 Overnight Testing
---

Tests not only **run at every commit/pull request**.

Furthermore, I've extended the CI process
to automatically run an **overnight testing** in GitHub Actions (for all branches).

```yml
on:
  schedule:
    # Overnight: run tests every Friday at 23:00 UTC
    - cron: "0 23 * * 5"
```

Let's break the `cron` field down:
```yml
cron <minute> <hour> <day-of-month> <day-of-week (sunday=0)>
```
