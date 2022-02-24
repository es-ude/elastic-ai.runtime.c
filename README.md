# CMake Unity Example Project

## Setup 

```bash
git submodule update --init --recursive
```

```bash
mkdir build
```

## Build and Run Tests

```
cd build
```

```bash
cmake -DTARGET_GROUP=test .
```

```bash
make test
```

## Build and Run Production

```bash
cd build
```

Target group production is currently default.

```bash
cmake .
```

or

```bash
cmake -DTARGET_GROUP=production .
```

```bash
make all
```

and execute it with

```bash
./bin/release
```
