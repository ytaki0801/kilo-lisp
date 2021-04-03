# Kilo embedding LISP interpreter

These are sample codes for embedding various LISP interpreters to [Kilo](https://github.com/antirez/kilo), with operations of last S-expression evaluating and parentheses matching.
The source code to compile Kilo is supposed to be from a tutorial site, [Build Your Own Text Editor](https://viewsourcecode.org/snaptoken/kilo/) Step 130, which is copied as `kilo-BYOTE130.c` in this repository.

![demo](https://github.com/ytaki0801/kilo-lisp/blob/master/kilo-lisp.gif)

## How to Compile and Use

Before compiling, you must install libraries of each LISP interpreter, except for no interpreter or `jmc`. For example, `sudo apt install gauche-dev` is needed to use Gauche on Debian package management system.

After install of the LISP interpreter libraries, just type the following in this repository to compile and use. Parentheses matching is done automatically, C-e is to eval-last-sexp, [C-s is to save and C-q is to quit](https://github.com/antirez/kilo).

```
$ make <name-of-interpreter or blank>
$ ./kilo-lisp <filename>
```

Now the following LISP interpreters are supposed as a `name-of-interpreter`, although most ones did not support many data types of a return value and/or eval error handling.

* `blank`: No interpreter
	* Just for parentheses matching and echo of last S-expression.

* **`jmc`**: McCarthy's Original Lisp
	* Implemented in C by [ytaki0801](https://github.com/ytaki0801) for McCarthy's 1960 paper definitions, derived from [Paul Graham's Common Lisp implementation](http://www.paulgraham.com/lispcode.html), without eval error handling. See `sample.jmc` for sample codes.
	* Docker image: `docker run --rm -it ytaki0801/kilo-jmclisp`

* **`gauche`**: [Gauche - A Scheme Implementation](http://practical-scheme.net/gauche/) >= 0.9.6
	* Mostly supported except some data types to eval, vector expression for example.

* **`guile`**: [GNU Guile](https://www.gnu.org/software/guile/) >= 3.0.4
	* Only supported for number to eval and no eval error handling.

* **`chibi`**: [Chibi-Scheme](http://synthcode.com/wiki/chibi-scheme) >= 0.9.1
	* Only supported for number to eval.

* **`ecl`**: [Embeddable Common Lisp](https://common-lisp.net/project/ecl/) >= 16.1.3
	* No eval error handling.

# License

* Kilo: [BSD 2-Clause "Simplified" License](https://github.com/snaptoken/kilo-src/blob/master/LICENSE)

* Source codes in this repository except Kilo: [Public Domain by CC0, Creative Commons Zero v1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)

* Gauche: https://github.com/shirok/Gauche/blob/master/COPYING

* GNU Guile: [GNU Lesser General Public Licence 3.0](http://www.gnu.org/licenses/lgpl.html)

* Chibi-Scheme: https://github.com/ashinn/chibi-scheme/blob/master/COPYING

* Embeddable Common Lisp: https://common-lisp.net/project/ecl/posts/ECL-license.html
