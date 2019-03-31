# Calculating Prime numbers

### How to run

```
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ./../
make
cd ..
python3 runScript.py 10
```

### Structure of the config file
```
err_abs: <double>
err_rel: <double>
num_threads: <int>
x_start: <double>
x_end: <double>
y_start: <double>
y_end: <double>
```