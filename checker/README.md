# 🧪 Python Checker for PCLP2 Homework 1

## Prerequisites

Install `valgrind`:

```sh
sudo apt update && sudo apt install -y valgrind
```

> The above command is specific to Ubuntu/Debian systems.


```sh
cd checker/
chmod +x ./coding-style/cs.sh
```

> To run the tests, you must be in `checker/` directory.


## Running tests with automated checker

```sh
python3 checker.py --help      			  # show this help message and exit
python3 checker.py --valgrind  			  # Run tests with `valgrind`. Tests may take longer to finish.
python3 checker.py --output    			  # Create output files for the selected tests in `output` directory.
python3 checker.py --clear     			  # Run tests for `clear` command.
python3 checker.py --analyse   			  # Run tests for `analyse` command.
python3 checker.py --print     			  # Run tests for `print` command.
python3 checker.py --all       			  # Run all tests.
python3 checker.py --all --valgrind       # Run all tests with valgrind.
```


## Run specific tests with `valgrind` (Memory Leak Check)

```sh
python3 checker.py --print --valgrind
python3 checker.py --clear --valgrind
python3 checker.py --analyse --valgrind
```




