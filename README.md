# ccpathnames

[![Build Status](https://travis-ci.org/marcomaggi/ccpathnames.svg?branch=master)](https://travis-ci.org/marcomaggi/ccpathnames)
[![codecov](https://codecov.io/gh/marcomaggi/ccpathnames/branch/master/graph/badge.svg)](https://codecov.io/gh/marcomaggi/ccpathnames)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/eeff156606fe41ce8bb56468a9c8106b)](https://www.codacy.com/app/marcomaggi/ccpathnames?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=marcomaggi/ccpathnames&amp;utm_campaign=Badge_Grade)

## Introduction

This package installs a C11 language library implementing Unix pathnames
manipulation functions.

The  library  targets  POSIX  systems.   This  package  depends  on  the
packages:  CCExceptions, at  least  version 0.10.0;  CCMemory, at  least
version  0.3.0; CCStructs,  at least  version  0.4.0.  To  run the  test
suite: this package  depends upon the package CCTests,  at least version
0.5.0.

The package uses the GNU Autotools and it is tested, using Travis CI, on
both Ubuntu GNU+Linux  systems and OS X systems.  The  package relies on
`pkg-config` to find the dependencies installed on the system.

## License

Copyright (c) 2018, 2019, 2020 Marco Maggi <mrc.mgg@gmail.com><br/>
All rights reserved.

This program is free software: you  can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free  Software Foundation, either version  3 of the License,  or (at
your option) any later version.

This program  is distributed  in the  hope that it  will be  useful, but
WITHOUT   ANY   WARRANTY;  without   even   the   implied  warranty   of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Install

To install from a proper release tarball, do this:

```
$ cd ccpathnames-0.1.0
$ mkdir build
$ cd build
$ ../configure
$ make
$ make check
$ make install
```

to inspect the available configuration options:

```
$ ../configure --help
```

The Makefile is designed to allow parallel builds, so we can do:

```
$ make -j4 all && make -j4 check
```

which,  on  a  4-core  CPU,   should  speed  up  building  and  checking
significantly.

The Makefile supports the DESTDIR  environment variable to install files
in a temporary location, example: to see what will happen:

```
$ make -n install DESTDIR=/tmp/ccpathnames
```

to really do it:

```
$ make install DESTDIR=/tmp/ccpathnames
```

After the  installation it is  possible to verify the  installed library
against the test suite with:

```
$ make installcheck
```

From a repository checkout or snapshot  (the ones from the Github site):
we  must install  the GNU  Autotools  (GNU Automake,  GNU Autoconf,  GNU
Libtool), then  we must first run  the script `autogen.sh` from  the top
source directory, to generate the needed files:

```
$ cd ccpathnames
$ sh autogen.sh

```

notice  that  `autogen.sh`  will   run  the  programs  `autoreconf`  and
`libtoolize`; the  latter is  selected through the  environment variable
`LIBTOOLIZE`,  whose  value  can  be  customised;  for  example  to  run
`glibtoolize` rather than `libtoolize` we do:

```
$ LIBTOOLIZE=glibtoolize sh autogen.sh
```

After this  the procedure  is the same  as the one  for building  from a
proper release tarball, but we have to enable maintainer mode:

```
$ ../configure --enable-maintainer-mode [options]
$ make
$ make check
$ make install
```

## Usage

Read the documentation generated from  the Texinfo sources.  The package
installs the documentation  in Info format; we can  generate and install
documentation in HTML format by running:

```
$ make html
$ make install-html
```

## Credits

The  stuff was  written by  Marco Maggi.   If this  package exists  it's
because of the great GNU software tools that he uses all the time.

## Bugs, vulnerabilities and contributions

Bug  and vulnerability  reports are  appreciated, all  the vulnerability
reports  are  public; register  them  using  the  Issue Tracker  at  the
project's GitHub  site.  For  contributions and  patches please  use the
Pull Requests feature at the project's GitHub site.

## Resources

The latest release of this package can be downloaded from:

[https://bitbucket.org/marcomaggi/ccpathnames/downloads](https://bitbucket.org/marcomaggi/ccpathnames/downloads)

development takes place at:

[http://github.com/marcomaggi/ccpathnames/](http://github.com/marcomaggi/ccpathnames/)

and as backup at:

[https://bitbucket.org/marcomaggi/ccpathnames/](https://bitbucket.org/marcomaggi/ccpathnames/)

the documentation is available online:

[http://marcomaggi.github.io/docs/ccpathnames.html](http://marcomaggi.github.io/docs/ccpathnames.html)

the GNU Project software can be found here:

[http://www.gnu.org/](http://www.gnu.org/)

the package CCExceptions is available at:

[https://github.com/marcomaggi/ccexceptions/](https://github.com/marcomaggi/ccexceptions/)

the package CCMemory is available at:

[https://github.com/marcomaggi/ccmemory/](https://github.com/marcomaggi/ccmemory/)

the package CCStructs is available at:

[https://github.com/marcomaggi/ccstructs/](https://github.com/marcomaggi/ccstructs/)

the package CCTests is available at:

[https://github.com/marcomaggi/cctests/](https://github.com/marcomaggi/cctests/)

## Badges and static analysis

### Travis CI

Travis CI is  a hosted, distributed continuous  integration service used
to build and test software projects  hosted at GitHub.  We can find this
project's dashboard at:

[https://travis-ci.org/marcomaggi/ccpathnames](https://travis-ci.org/marcomaggi/ccpathnames)

Usage of this  service is configured through the  file `.travis.yml` and
additional scripts are under the directory `meta/travis-ci`.

### Clang's Static Analyzer

The Clang Static Analyzer is a source code analysis tool that finds bugs
in C, C++, and Objective-C programs.  It is distributed along with Clang
and we can find it at:

[http://clang-analyzer.llvm.org/](http://clang-analyzer.llvm.org/)

Usage of this  service is implemented with make rules;  see the relevant
section in the file `Makefile.am`.

### Codecov

Codecov is a service providing code  coverage reports.  We can find this
project's dashboard at:

[https://codecov.io/gh/marcomaggi/ccpathnames](https://codecov.io/gh/marcomaggi/ccpathnames)

Usage of  this service is  implemented through direct  interface between
GitHub and Codecov  sites; it configured through  the file `codecov.yml`
and appropriate entries in Travis CI's matrix of builds.

### Codacy

Codacy is  an online service  providing code  review.  We can  find this
project's dashboard at:

[https://www.codacy.com/app/marcomaggi/ccpathnames](https://www.codacy.com/app/marcomaggi/ccpathnames)

Usage of this service is  implemented through direct integration between
GitHub and Codacy sites.

