# PHONETIC

A C++ library for working with the CMU Pronouncing Dictionary. Now with Python bindings. 

## Intro

The [CMU Pronouncing Dictionary](http://www.speech.cs.cmu.edu/cgi-bin/cmudict#about) (CMUdict) is

> an open-source machine-readable pronunciation dictionary for North American English that contains over 134,000 words and their pronunciations.

There's various tooling built around it, e.g. [this lovely Python library](https://github.com/aparrish/pronouncingpy). But I was working in C++ and WebAssembly, and ended up writing my own interface for using CMUdict.

## Build

Includes CMUdict as a sub-module. To load that data use:

```
git submodule update --init --recursive
```

To compile this project with CMake:

```
mkdir build
cd build
cmake ..
make
```

Which will also build a `build/tests/test_phonetic` Catch-2 test file.

This library can also be compiled to WebAssembly using Emscripten:
```
mdkir build
cd build
emcmake cmake ..
emmake make
```

This will generate a `phonetic.js`, `phonetic.wasm`, and `phonetic.data`. Using these you'll be able to call any of the `Phonetic` class methods straight from Javascript.

Thanks to @nwoods, this project now has python bindings. From the project root directory run:

```
pip install . 
```

You'll then be able to run all of the class methods from inside the tidy confines of Python.

## Usage

Use this library to convert English words into:
1. Possible pronunciations, as [ARPABET](https://en.wikipedia.org/wiki/ARPABET#:~:text=ARPABET%20(also%20spelled%20ARPAbet)%20is,distinct%20sequences%20of%20ASCII%20characters.) which encodes IPA into two letter ASCII sequences. 
2. Possible patterns of syllabic stress, as strings of numbers `0`, `1`, & `2`, where `0` is unstressed, `1` is primary stress, and `2` is secondary stress.
3. Possible syllable counts (counting the number of vowel phones).

Note that if a word has multiple pronunciations, stress patterns, or syllable counts, all of these will be returned. 

The tests (`test/test_phonetic.cpp`) offers a chance to see all of the methods in action.

## Future Features

- [ ] Search by phones.
- [ ] Search by stress.
- [ ] Search by syllable count.
- [ ] More useful WebAssembly error handling.

